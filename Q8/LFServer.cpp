#include "LFServer.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

LFServer::LFServer(int serverPort, int workers) 
    : port(serverPort), serverSocket(-1), running(false), 
      numWorkers(workers), currentLeader(-1), totalConnections(0), 
      totalRequests(0), leaderPromotions(0) {
}

LFServer::~LFServer() {
    stop();
}

void LFServer::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Cannot create socket");
    }
    
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Bind failed");
    }
    
    if (listen(serverSocket, 10) < 0) {
        close(serverSocket);
        throw std::runtime_error("Listen failed");
    }
    
    running = true;
    
    // Start worker threads (followers)
    for (int i = 0; i < numWorkers; i++) {
        workerThreads.emplace_back(&LFServer::workerThread, this, i);
    }
    
    std::cout << "Q8 Leader-Follower Server running on port " << port << std::endl;
    std::cout << "Workers: " << numWorkers << std::endl;
    std::cout << "Leader-Follower Pattern: Only ONE leader accepts connections at a time" << std::endl;
    
    // Main acceptor thread
    acceptorThread();
}

void LFServer::stop() {
    running = false;
    if (serverSocket >= 0) {
        close(serverSocket);
    }
    
    leaderCondition.notify_all();
    
    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void LFServer::acceptorThread() {
    while (running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            if (running) {
                std::cerr << "Accept failed" << std::endl;
            }
            continue;
        }
        
        int clientId = ++totalConnections;
        logRequest(clientId, "Connection accepted");
        
        // Send welcome message
        std::string welcome = "=== Q8 Leader-Follower Server ===\n"
                             "Format: vertices edges edge1 edge2 ...\n"
                             "Server runs ALL 4 algorithms on each graph\n"
                             "Example: 3 3 0-1 1-2 2-0\n"
                             "Commands: help, stats, quit\n> ";
        sendResponse(clientSocket, welcome);
        
        // Handle client in separate thread for this connection
        std::thread([this, clientSocket, clientId]() {
            char buffer[1024];
            while (running) {
                memset(buffer, 0, sizeof(buffer));
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                
                if (bytesRead <= 0) {
                    logRequest(clientId, "Disconnected");
                    break;
                }
                
                std::string input(buffer);
                input.erase(input.find_last_not_of("\r\n") + 1);
                
                if (input == "quit" || input == "exit") {
                    sendResponse(clientSocket, "Goodbye!\n");
                    break;
                } else if (input == "help") {
                    std::string help = "=== Q8 Leader-Follower Server ===\n"
                                     "Format: vertices edges edge1 edge2 ...\n"
                                     "Server executes ALL 4 algorithms:\n"
                                     "- Euler Circuit\n- Max Clique\n- MST Weight\n- Hamilton Circuit\n"
                                     "Commands: help, stats, quit\n> ";
                    sendResponse(clientSocket, help);
                } else if (input == "stats") {
                    std::lock_guard<std::mutex> lock(statsMutex);
                    std::ostringstream stats;
                    stats << "=== Leader-Follower Statistics ===\n";
                    stats << "Total Connections: " << totalConnections << "\n";
                    stats << "Total Requests: " << totalRequests << "\n";
                    stats << "Leader Promotions: " << leaderPromotions << "\n";
                    stats << "Active Workers: " << numWorkers << "\n";
                    stats << "Current Leader: " << currentLeader << "\n";
                    stats << "==================================\n> ";
                    sendResponse(clientSocket, stats.str());
                } else if (!input.empty()) {
                    // Add request to queue for Leader-Follower processing
                    auto request = std::make_shared<ClientRequest>(clientSocket, clientId, input);
                    {
                        std::lock_guard<std::mutex> lock(leaderMutex);
                        requestQueue.push(request);
                        totalRequests++;
                    }
                    leaderCondition.notify_one(); // Wake up the leader
                } else {
                    sendResponse(clientSocket, "Enter graph data. Type 'help'.\n> ");
                }
            }
            close(clientSocket);
        }).detach();
    }
}

void LFServer::workerThread(int workerId) {
    while (running) {
        std::unique_lock<std::mutex> lock(leaderMutex);
        
        // Wait to become leader or for work
        leaderCondition.wait(lock, [this, workerId] {
            return !running || (!requestQueue.empty() && currentLeader == workerId) || currentLeader == -1;
        });
        
        if (!running) break;
        
        // Promote to leader if no leader exists
        if (currentLeader == -1 && !requestQueue.empty()) {
            currentLeader = workerId;
            {
                std::lock_guard<std::mutex> statsLock(statsMutex);
                leaderPromotions++;
            }
            logRequest(-1, "Worker " + std::to_string(workerId) + " promoted to LEADER");
        }
        
        // Process request as leader
        if (currentLeader == workerId && !requestQueue.empty()) {
            auto request = requestQueue.front();
            requestQueue.pop();
            
            // Immediately promote new leader before processing
            promoteNewLeader();
            lock.unlock();
            
            // Process the request (now as follower)
            logRequest(request->clientId, "Processing request (Worker " + std::to_string(workerId) + ")");
            
            try {
                Graph graph = parseGraph(request->graphData);
                AllAlgorithmsResult results = executeAllAlgorithms(graph, request->clientId);
                std::string response = formatAllResults(results, graph) + "\n> ";
                sendResponse(request->clientSocket, response);
                
                logRequest(request->clientId, "Completed all 4 algorithms in " + std::to_string(results.totalTime) + "us");
                
            } catch (const std::exception& e) {
                std::string errorResponse = "Error: " + std::string(e.what()) + "\n> ";
                sendResponse(request->clientSocket, errorResponse);
                logRequest(request->clientId, "Error: " + std::string(e.what()));
            }
        }
    }
}

void LFServer::promoteNewLeader() {
    if (!requestQueue.empty()) {
        // Find next worker to be leader (round-robin)
        int nextLeader = (currentLeader + 1) % numWorkers;
        currentLeader = nextLeader;
        {
            std::lock_guard<std::mutex> statsLock(statsMutex);
            leaderPromotions++;
        }
        logRequest(-1, "Promoted Worker " + std::to_string(nextLeader) + " to LEADER");
        leaderCondition.notify_all();
    } else {
        currentLeader = -1; // No leader needed when no work
    }
}

Graph LFServer::parseGraph(const std::string& input) {
    std::istringstream iss(input);
    int vertices, edges;
    
    if (!(iss >> vertices >> edges)) {
        throw std::invalid_argument("Invalid format. Expected: vertices edges edge1 edge2 ...");
    }
    
    if (vertices <= 0 || edges < 0) {
        throw std::invalid_argument("Invalid vertices or edges count");
    }
    
    Graph graph(vertices, false); // All algorithms work on undirected
    
    for (int i = 0; i < edges; i++) {
        std::string edge;
        if (!(iss >> edge)) {
            throw std::invalid_argument("Missing edges. Expected " + std::to_string(edges) + " edges");
        }
        
        size_t dashPos = edge.find('-');
        if (dashPos == std::string::npos) {
            throw std::invalid_argument("Invalid edge format. Use u-v (e.g., 0-1)");
        }
        
        int u = std::stoi(edge.substr(0, dashPos));
        int v = std::stoi(edge.substr(dashPos + 1));
        
        if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
            throw std::invalid_argument("Invalid vertex. Must be 0-" + std::to_string(vertices-1));
        }
        
        graph.addEdge(u, v);
    }
    
    return graph;
}

AllAlgorithmsResult LFServer::executeAllAlgorithms(const Graph& graph, int clientId) {
    AllAlgorithmsResult results(clientId);
    
    results.clientId = clientId;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Execute all 4 algorithms as required by Q8
    auto eulerAlg = AlgorithmFactory::createAlgorithm("euler");
    results.euler = eulerAlg->execute(graph);
    
    auto cliqueAlg = AlgorithmFactory::createAlgorithm("clique");
    results.clique = cliqueAlg->execute(graph);
    
    auto mstAlg = AlgorithmFactory::createAlgorithm("mst");
    results.mst = mstAlg->execute(graph);
    
    auto hamiltonAlg = AlgorithmFactory::createAlgorithm("hamilton");
    results.hamilton = hamiltonAlg->execute(graph);
    
    auto end = std::chrono::high_resolution_clock::now();
    results.totalTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    return results;
}

std::string LFServer::formatAllResults(const AllAlgorithmsResult& results, const Graph& graph) {
    std::ostringstream output;
    output << "\n=== ALL 4 ALGORITHMS RESULTS (Q8) ===\n";
    output << "Graph: " << graph.getVertexCount() << " vertices, " 
           << graph.getEdgeCount() << " edges\n\n";
    
    output << "1. EULER CIRCUIT: ";
    if (results.euler.success) {
        output << results.euler.result << " (" << results.euler.executionTime << "us)\n";
    } else {
        output << "ERROR - " << results.euler.error << "\n";
    }
    
    output << "2. MAX CLIQUE: ";
    if (results.clique.success) {
        output << results.clique.result << " (" << results.clique.executionTime << "us)\n";
    } else {
        output << "ERROR - " << results.clique.error << "\n";
    }
    
    output << "3. MST WEIGHT: ";
    if (results.mst.success) {
        output << results.mst.result << " (" << results.mst.executionTime << "us)\n";
    } else {
        output << "ERROR - " << results.mst.error << "\n";
    }
    
    output << "4. HAMILTON CIRCUIT: ";
    if (results.hamilton.success) {
        output << results.hamilton.result << " (" << results.hamilton.executionTime << "us)\n";
    } else {
        output << "ERROR - " << results.hamilton.error << "\n";
    }
    
    output << "\nTotal execution time: " << results.totalTime << "us";
    output << "\n=====================================";
    
    return output.str();
}

void LFServer::sendResponse(int clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), response.length(), 0);
}

void LFServer::logRequest(int clientId, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tm);
    
    if (clientId == -1) {
        std::cout << "[" << timeStr << "] [LF-SYSTEM] " << message << std::endl;
    } else {
        std::cout << "[" << timeStr << "] [Client " << clientId << "] " << message << std::endl;
    }
}