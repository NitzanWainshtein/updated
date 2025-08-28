#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Graph.h"
#include "AlgorithmFactory.h"

void handleClient(int clientSocket) {
    std::string welcome = "=== Q7 Multi-Algorithms Server ===\n"
                         "Strategy + Factory Patterns\n"
                         "Format: algorithm vertices edges edge1 edge2 ...\n"
                         "Available: euler, clique, mst, hamilton\n"
                         "Example: euler 3 3 0-1 1-2 2-0\n"
                         "Commands: help, quit\n> ";
    
    send(clientSocket, welcome.c_str(), welcome.length(), 0);
    
    char buffer[1024];
    while (true) {
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
            response = "Enter algorithm request. Type 'help'.\n> ";
        } else {
            // Parse and execute algorithm
            try {
                std::istringstream iss(input);
                std::string alg;
                int vertices, edges;
                
                if (!(iss >> alg >> vertices >> edges)) {
                    response = "Error: Format: algorithm vertices edges edge1 edge2 ...\n> ";
                } else if (!AlgorithmFactory::isValidAlgorithm(alg)) {
                    response = "Error: Unknown algorithm. Available: euler, clique, mst, hamilton\n> ";
                } else {
                    Graph graph(vertices, false);
                    
                    // Add edges
                    for (int i = 0; i < edges; i++) {
                        std::string edge;
                        if (!(iss >> edge)) {
                            response = "Error: Missing edges\n> ";
                            break;
                        }
                        
                        size_t dashPos = edge.find('-');
                        if (dashPos != std::string::npos) {
                            int u = std::stoi(edge.substr(0, dashPos));
                            int v = std::stoi(edge.substr(dashPos + 1));
                            graph.addEdge(u, v);
                        }
                    }
                    
                    if (response.empty()) {
                        // Execute algorithm using Factory pattern
                        auto algorithm = AlgorithmFactory::createAlgorithm(alg);
                        auto result = algorithm->execute(graph);
                        
                        response = "\n=== " + alg + " RESULT ===\n";
                        if (result.success) {
                            response += "Result: " + result.result + "\n";
                            response += "Time: " + std::to_string(result.executionTime) + " us\n";
                        } else {
                            response += "Error: " + result.error + "\n";
                        }
                        response += "================\n> ";
                    }
                }
            } catch (const std::exception& e) {
                response = "Error: " + std::string(e.what()) + "\n> ";
            }
        }
        
        send(clientSocket, response.c_str(), response.length(), 0);
    }
    
    close(clientSocket);
}

int main(int argc, char* argv[]) {
    int port = 8081;
    
    if (argc == 2) {
        port = std::stoi(argv[1]);
    }
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    
    std::cout << "Q7 Multi-Algorithms Server running on port " << port << std::endl;
    std::cout << "Design Patterns: Strategy + Factory" << std::endl;
    std::cout << "Available algorithms: euler, clique, mst, hamilton" << std::endl;
    
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocket >= 0) {
            std::thread clientThread(handleClient, clientSocket);
            clientThread.detach();
        }
    }
    
    return 0;
}
