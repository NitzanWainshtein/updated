#include "PipelineServer.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

PipelineServer::PipelineServer(int serverPort) 
    : port(serverPort), serverSocket(-1), running(false), 
      totalConnections(0), totalRequests(0), pipelineErrors(0) {
    
    // Initialize 4 Active Objects for pipeline stages
    requestReceiver = std::make_unique<ActiveObject>("Stage1-RequestReceiver");
    graphProcessor = std::make_unique<ActiveObject>("Stage2-GraphProcessor");
    algorithmExecutor = std::make_unique<ActiveObject>("Stage3-AlgorithmExecutor");
    responseFormatter = std::make_unique<ActiveObject>("Stage4-ResponseFormatter");
}

PipelineServer::~PipelineServer() {
    stop();
}

void PipelineServer::start() {
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
    
    // Start all Active Objects
    requestReceiver->start();
    graphProcessor->start();
    algorithmExecutor->start();
    responseFormatter->start();
    
    std::cout << "Q9 Pipeline Server running on port " << port << std::endl;
    std::cout << "Pipeline: 4 Active Objects (Request->Graph->Algorithms->Response)" << std::endl;
    std::cout << "Each request flows through all 4 stages asynchronously" << std::endl;
    
    acceptorThread();
}

void PipelineServer::stop() {
    running = false;
    if (serverSocket >= 0) {
        close(serverSocket);
    }
    
    if (requestReceiver) requestReceiver->stop();
    if (graphProcessor) graphProcessor->stop();
    if (algorithmExecutor) algorithmExecutor->stop();
    if (responseFormatter) responseFormatter->stop();
}

void PipelineServer::acceptorThread() {
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
        logRequest(clientId, "Connection accepted - entering pipeline");
        
        std::thread([this, clientSocket, clientId]() {
            handleClientConnection(clientSocket, clientId);
        }).detach();
    }
}

void PipelineServer::handleClientConnection(int clientSocket, int clientId) {
    std::string welcome = "=== Q9 Pipeline Server (4 Active Objects) ===\n"
                         "Format: vertices edges edge1 edge2 ...\n"
                         "Pipeline stages: Request->Graph->Algorithms->Response\n"
                         "All 4 algorithms executed asynchronously\n"
                         "Commands: help, stats, quit\n> ";
    sendResponse(clientSocket, welcome);
    
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
            std::string help = welcome;
            sendResponse(clientSocket, help);
        } else if (input == "stats") {
            auto stats = getStats();
            std::ostringstream statsStr;
            statsStr << "\n=== Pipeline Statistics ===\n";
            statsStr << "Total Requests: " << stats.totalRequests << "\n";
            statsStr << "Pipeline Errors: " << stats.totalErrors << "\n";
            statsStr << "Stage 1 (Request): " << stats.stage1Processed << " processed, " << stats.stage1Queue << " queued\n";
            statsStr << "Stage 2 (Graph): " << stats.stage2Processed << " processed, " << stats.stage2Queue << " queued\n";
            statsStr << "Stage 3 (Algorithms): " << stats.stage3Processed << " processed, " << stats.stage3Queue << " queued\n";
            statsStr << "Stage 4 (Response): " << stats.stage4Processed << " processed, " << stats.stage4Queue << " queued\n";
            statsStr << "===========================\n> ";
            sendResponse(clientSocket, statsStr.str());
        } else if (!input.empty()) {
            // Create request and send to pipeline Stage 1
            auto request = std::make_shared<PipelineRequest>(clientSocket, clientId, input);
            totalRequests++;
            
            logRequest(clientId, "Request entering Stage 1 (RequestReceiver)");
            requestReceiver->enqueue([this, request]() {
                stage1_receiveRequest(request);
            });
        } else {
            sendResponse(clientSocket, "Enter graph data. Type 'help'.\n> ");
        }
    }
    
    close(clientSocket);
}

// STAGE 1: Request Receiver - Parse and validate input
void PipelineServer::stage1_receiveRequest(std::shared_ptr<PipelineRequest> request) {
    try {
        logRequest(request->clientId, "Stage 1: Processing request");
        
        Graph graph = parseGraphInput(request->inputData);
        
        logRequest(request->clientId, "Stage 1 -> Stage 2");
        
        // Pass to Stage 2
        auto processedGraph = std::make_shared<ProcessedGraph>();
        processedGraph->clientSocket = request->clientSocket;
        processedGraph->clientId = request->clientId;
        processedGraph->graph = graph;
        processedGraph->startTime = request->startTime;
        
        graphProcessor->enqueue([this, processedGraph]() {
            stage2_processGraph(processedGraph);
        });
        
    } catch (const std::exception& e) {
        pipelineErrors++;
        std::string errorResponse = formatError(e.what(), request->clientId) + "\n> ";
        sendResponse(request->clientSocket, errorResponse);
        logRequest(request->clientId, "Stage 1 error: " + std::string(e.what()));
    }
}

// STAGE 2: Graph Processor - Additional graph processing/validation
void PipelineServer::stage2_processGraph(std::shared_ptr<ProcessedGraph> graph) {
    try {
        logRequest(graph->clientId, "Stage 2: Processing graph");
        
        // Additional graph validation
        if (graph->graph.getVertexCount() == 0) {
            throw std::invalid_argument("Empty graph");
        }
        
        logRequest(graph->clientId, "Stage 2 -> Stage 3");
        
        // Pass to Stage 3 - prepare algorithm results structure
        auto results = std::make_shared<AlgorithmResults>();
        results->clientSocket = graph->clientSocket;
        results->clientId = graph->clientId;
        results->graph = graph->graph;
        results->startTime = graph->startTime;
        
        algorithmExecutor->enqueue([this, results]() {
            stage3_executeAlgorithms(results);
        });
        
    } catch (const std::exception& e) {
        pipelineErrors++;
        std::string errorResponse = formatError(e.what(), graph->clientId) + "\n> ";
        sendResponse(graph->clientSocket, errorResponse);
        logRequest(graph->clientId, "Stage 2 error: " + std::string(e.what()));
    }
}

// STAGE 3: Algorithm Executor - Execute all 4 algorithms
void PipelineServer::stage3_executeAlgorithms(std::shared_ptr<AlgorithmResults> results) {
    try {
        logRequest(results->clientId, "Stage 3: Executing all 4 algorithms");
        
        // Execute all 4 algorithms as required by Q9
        auto eulerAlg = AlgorithmFactory::createAlgorithm("euler");
        results->euler = eulerAlg->execute(results->graph);
        
        auto cliqueAlg = AlgorithmFactory::createAlgorithm("clique");
        results->clique = cliqueAlg->execute(results->graph);
        
        auto mstAlg = AlgorithmFactory::createAlgorithm("mst");
        results->mst = mstAlg->execute(results->graph);
        
        auto hamiltonAlg = AlgorithmFactory::createAlgorithm("hamilton");
        results->hamilton = hamiltonAlg->execute(results->graph);
        
        logRequest(results->clientId, "Stage 3 -> Stage 4");
        
        // Pass to Stage 4
        responseFormatter->enqueue([this, results]() {
            stage4_formatResponse(results);
        });
        
    } catch (const std::exception& e) {
        pipelineErrors++;
        std::string errorResponse = formatError(e.what(), results->clientId) + "\n> ";
        sendResponse(results->clientSocket, errorResponse);
        logRequest(results->clientId, "Stage 3 error: " + std::string(e.what()));
    }
}

// STAGE 4: Response Formatter - Format and send response
void PipelineServer::stage4_formatResponse(std::shared_ptr<AlgorithmResults> results) {
    try {
        logRequest(results->clientId, "Stage 4: Formatting and sending response");
        
        std::string response = formatPipelineResponse(*results) + "\n> ";
        sendResponse(results->clientSocket, response);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - results->startTime);
        
        logRequest(results->clientId, "Pipeline completed in " + std::to_string(totalTime.count()) + "us");
        
    } catch (const std::exception& e) {
        pipelineErrors++;
        std::string errorResponse = formatError(e.what(), results->clientId) + "\n> ";
        sendResponse(results->clientSocket, errorResponse);
        logRequest(results->clientId, "Stage 4 error: " + std::string(e.what()));
    }
}

Graph PipelineServer::parseGraphInput(const std::string& input) {
    std::istringstream iss(input);
    int vertices, edges;
    
    if (!(iss >> vertices >> edges)) {
        throw std::invalid_argument("Invalid format. Expected: vertices edges edge1 edge2 ...");
    }
    
    if (vertices <= 0 || edges < 0) {
        throw std::invalid_argument("Invalid vertices or edges count");
    }
    
    Graph graph(vertices, false);
    
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

std::string PipelineServer::formatPipelineResponse(const AlgorithmResults& results) {
    std::ostringstream output;
    output << "\n=== PIPELINE RESULTS (4 Active Objects) ===\n";
    output << "Graph: " << results.graph.getVertexCount() << " vertices, " 
           << results.graph.getEdgeCount() << " edges\n\n";
    
    output << "1. EULER: ";
    output << (results.euler.success ? results.euler.result : "ERROR - " + results.euler.error);
    output << " (" << results.euler.executionTime << "us)\n";
    
    output << "2. CLIQUE: ";
    output << (results.clique.success ? results.clique.result : "ERROR - " + results.clique.error);
    output << " (" << results.clique.executionTime << "us)\n";
    
    output << "3. MST: ";
    output << (results.mst.success ? results.mst.result : "ERROR - " + results.mst.error);
    output << " (" << results.mst.executionTime << "us)\n";
    
    output << "4. HAMILTON: ";
    output << (results.hamilton.success ? results.hamilton.result : "ERROR - " + results.hamilton.error);
    output << " (" << results.hamilton.executionTime << "us)\n";
    
    output << "\nProcessed through 4-stage pipeline";
    output << "\n========================================";
    
    return output.str();
}

PipelineServer::PipelineStats PipelineServer::getStats() {
    PipelineStats stats;
    stats.totalRequests = totalRequests;
    stats.totalErrors = pipelineErrors;
    stats.stage1Processed = requestReceiver->getProcessedTaskCount();
    stats.stage2Processed = graphProcessor->getProcessedTaskCount();
    stats.stage3Processed = algorithmExecutor->getProcessedTaskCount();
    stats.stage4Processed = responseFormatter->getProcessedTaskCount();
    stats.stage1Queue = requestReceiver->getQueueSize();
    stats.stage2Queue = graphProcessor->getQueueSize();
    stats.stage3Queue = algorithmExecutor->getQueueSize();
    stats.stage4Queue = responseFormatter->getQueueSize();
    return stats;
}

void PipelineServer::sendResponse(int clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), response.length(), 0);
}

void PipelineServer::logRequest(int clientId, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tm);
    
    std::cout << "[" << timeStr << "] [Client " << clientId << "] " << message << std::endl;
}

std::string PipelineServer::formatError(const std::string& error, int clientId) {
    return "\n=== PIPELINE ERROR (Client " + std::to_string(clientId) + ") ===\n" + 
           error + "\n==============================";
}