#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class EulerClient {
private:
    int clientSocket;
    std::string host;
    int port;
    
public:
    EulerClient(const std::string& h = "localhost", int p = 8080) 
        : clientSocket(-1), host(h), port(p) {}
    
    bool connect() {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Cannot create socket" << std::endl;
            return false;
        }
        
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
            if (host == "localhost") {
                inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
            } else {
                std::cerr << "Invalid address: " << host << std::endl;
                close(clientSocket);
                return false;
            }
        }
        
        if (::connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Connection failed to " << host << ":" << port << std::endl;
            close(clientSocket);
            return false;
        }
        
        return true;
    }
    
    void run() {
        if (!connect()) {
            return;
        }
        
        std::cout << "Connected to Euler server at " << host << ":" << port << std::endl;
        std::cout << "Type 'quit' to exit" << std::endl;
        
        // Receive welcome message
        char buffer[2048];
        recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        std::cout << buffer;
        
        std::string input;
        while (true) {
            std::getline(std::cin, input);
            
            if (input.empty()) continue;
            
            // Send input to server
            send(clientSocket, input.c_str(), input.length(), 0);
            
            if (input == "quit" || input == "exit") {
                // Receive goodbye message
                memset(buffer, 0, sizeof(buffer));
                recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                std::cout << buffer;
                break;
            }
            
            // Receive response
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                std::cout << buffer;
            } else {
                std::cout << "Connection lost" << std::endl;
                break;
            }
        }
        
        close(clientSocket);
    }
    
    ~EulerClient() {
        if (clientSocket >= 0) {
            close(clientSocket);
        }
    }
};

int main(int argc, char* argv[]) {
    std::string host = "localhost";
    int port = 8080;
    
    if (argc > 3) {
        std::cerr << "Usage: " << argv[0] << " [host] [port]" << std::endl;
        return 1;
    }
    
    if (argc >= 2) {
        host = argv[1];
    }
    
    if (argc == 3) {
        try {
            port = std::stoi(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid port: " << argv[2] << std::endl;
            return 1;
        }
    }
    
    EulerClient client(host, port);
    client.run();
    
    return 0;
}