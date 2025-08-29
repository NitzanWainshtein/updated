#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include "PipelineServer.h"

PipelineServer* globalServer = nullptr;

void signalHandler(int) {
    std::cout << "\nShutting down pipeline server..." << std::endl;
    if (globalServer) {
        globalServer->stop();
    }
}

int main(int argc, char* argv[]) {
    int port = 8083;
    
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
        
        PipelineServer server(port);
        globalServer = &server;
        
        std::cout << "Starting Q9 Pipeline Server..." << std::endl;
        std::cout << "Port: " << port << std::endl;
        std::cout << "Architecture: 4-Stage Pipeline with Active Objects" << std::endl;
        std::cout << "Stages: Request -> Graph -> Algorithms -> Response" << std::endl;
        std::cout << "Feature: All 4 algorithms executed asynchronously" << std::endl;
        std::cout << "Connect with: telnet localhost " << port << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl;
        
        server.start();
        
        while (server.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}