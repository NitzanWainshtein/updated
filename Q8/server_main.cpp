#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include "LFServer.h"

LFServer* globalServer = nullptr;

void signalHandler(int) {
    std::cout << "\nShutting down server..." << std::endl;
    if (globalServer) {
        globalServer->stop();
    }
}

int main(int argc, char* argv[]) {
    int port = 8082;
    int workers = 4;
    
    if (argc > 3) {
        std::cerr << "Usage: " << argv[0] << " [port] [workers]" << std::endl;
        return 1;
    }
    
    if (argc >= 2) {
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
    
    if (argc == 3) {
        try {
            workers = std::stoi(argv[2]);
            if (workers < 2 || workers > 10) {
                std::cerr << "Workers must be 2-10" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid workers count: " << argv[2] << std::endl;
            return 1;
        }
    }
    
    try {
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);
        
        LFServer server(port, workers);
        globalServer = &server;
        
        std::cout << "Starting Q8 Leader-Follower Server..." << std::endl;
        std::cout << "Port: " << port << std::endl;
        std::cout << "Workers: " << workers << std::endl;
        std::cout << "Pattern: Leader-Follower Threading" << std::endl;
        std::cout << "Feature: Executes ALL 4 algorithms on each graph" << std::endl;
        std::cout << "Connect with: telnet localhost " << port << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl;
        std::cout << std::endl;
        
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