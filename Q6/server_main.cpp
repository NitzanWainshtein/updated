#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <thread>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Graph.h"

class EulerServer {
private:
    int port;
    int serverSocket;
    bool running;
    
public:
    EulerServer(int p = 8080) : port(p), serverSocket(-1), running(false) {}
    
    void start() {
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
        
        if (listen(serverSocket, 5) < 0) {
            close(serverSocket);
            throw std::runtime_error("Listen failed");
        }
        
        running = true;
        std::cout << "Euler server running on port " << port << std::endl;
        
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
            
            std::thread clientThread(&EulerServer::handleClient, this, clientSocket);
            clientThread.detach();
        }
    }
    
    void stop() {
        running = false;
        if (serverSocket >= 0) {
            close(serverSocket);
        }
    }
    
private:
    void handleClient(int clientSocket) {
        std::string welcome = "=== Euler Circuit Server ===\n"
                             "Format: vertices edges edge1 edge2 ...\n"
                             "Example: 3 3 0-1 1-2 2-0\n"
                             "Commands: help, quit\n> ";
        
        send(clientSocket, welcome.c_str(), welcome.length(), 0);
        
        char buffer[1024];
        while (running) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            
            if (bytesRead <= 0) break;
            
            std::string input(buffer);
            input.erase(input.find_last_not_of("\r\n") + 1);
            
            std::string response;
            if (input == "quit" || input == "exit") {
                response = "Goodbye!\n";
                send(clientSocket, response.c_str(), response.length(), 0);
                break;
            } else if (input == "help") {
                response = welcome;
            } else if (input.empty()) {
                response = "Enter graph data. Type 'help' for format.\n> ";
            } else {
                response = processGraph(input) + "\n> ";
            }
            
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        
        close(clientSocket);
    }
    
    std::string processGraph(const std::string& input) {
        try {
            std::istringstream iss(input);
            int vertices, edges;
            
            if (!(iss >> vertices >> edges)) {
                return "Error: Invalid format. Expected: vertices edges edge1 edge2 ...";
            }
            
            if (vertices <= 0 || edges < 0) {
                return "Error: Invalid vertices or edges count";
            }
            
            Graph graph(vertices, false);
            
            for (int i = 0; i < edges; i++) {
                std::string edge;
                if (!(iss >> edge)) {
                    return "Error: Missing edges. Expected " + std::to_string(edges) + " edges";
                }
                
                size_t dashPos = edge.find('-');
                if (dashPos == std::string::npos) {
                    return "Error: Invalid edge format. Use u-v (e.g., 0-1)";
                }
                
                int u = std::stoi(edge.substr(0, dashPos));
                int v = std::stoi(edge.substr(dashPos + 1));
                
                if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
                    return "Error: Invalid vertex. Vertices must be 0-" + std::to_string(vertices-1);
                }
                
                graph.addEdge(u, v);
            }
            
            // Analyze Euler circuit
            bool hasEuler = graph.hasEulerCircuit();
            std::string result = "Graph: " + std::to_string(vertices) + " vertices, " + 
                               std::to_string(edges) + " edges\n";
            result += "Connected: " + std::string(graph.isConnected() ? "Yes" : "No") + "\n";
            result += "Euler circuit: " + std::string(hasEuler ? "YES" : "NO");
            
            if (hasEuler) {
                Graph tempGraph = graph;
                std::vector<int> circuit = tempGraph.findEulerCircuit();
                if (!circuit.empty() && circuit.size() <= 20) {
                    result += "\nCircuit: ";
                    for (size_t i = 0; i < circuit.size(); i++) {
                        result += std::to_string(circuit[i]);
                        if (i < circuit.size() - 1) result += " -> ";
                    }
                }
            }
            
            return result;
            
        } catch (const std::exception& e) {
            return "Error: " + std::string(e.what());
        }
    }
};

EulerServer* globalServer = nullptr;

void signalHandler(int signal) {
    std::cout << "\nShutting down..." << std::endl;
    if (globalServer) {
        globalServer->stop();
    }
}

int main(int argc, char* argv[]) {
    int port = 8080;
    
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        return 1;
    }
    
    if (argc == 2) {
        try {
            port = std::stoi(argv[1]);
            if (port < 1024 || port > 65535) {
                std::cerr << "Port must be 1024-65535" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid port: " << argv[1] << std::endl;
            return 1;
        }
    }
    
    try {
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);
        
        EulerServer server(port);
        globalServer = &server;
        
        std::cout << "Starting Euler server on port " << port << std::endl;
        std::cout << "Connect with: telnet localhost " << port << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl;
        
        server.start();
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}