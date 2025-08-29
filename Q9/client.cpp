#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    std::string host = "localhost";
    int port = 8083;

    if (argc > 3) {
        std::cerr << "Usage: " << argv[0] << " [host] [port]" << std::endl;
        return 1;
    }
    if (argc >= 2) host = argv[1];
    if (argc == 3) {
        try { port = std::stoi(argv[2]); }
        catch (...) { std::cerr << "Invalid port\n"; return 1; }
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { std::cerr << "Cannot create socket\n"; return 1; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
        if (host == "localhost") inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        else { std::cerr << "Invalid address\n"; close(sock); return 1; }
    }

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Connection failed to " << host << ":" << port << std::endl;
        close(sock);
        return 1;
    }

    char buf[4096];
    int n = recv(sock, buf, sizeof(buf)-1, 0);
    if (n > 0) { buf[n] = 0; std::cout << buf; }

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        send(sock, line.c_str(), line.size(), 0);
        if (line == "quit" || line == "exit") {
            memset(buf, 0, sizeof(buf));
            int r = recv(sock, buf, sizeof(buf)-1, 0);
            if (r > 0) std::cout << buf;
            break;
        }
        memset(buf, 0, sizeof(buf));
        int r = recv(sock, buf, sizeof(buf)-1, 0);
        if (r <= 0) { std::cout << "Connection lost\n"; break; }
        std::cout << buf;
    }

    close(sock);
    return 0;
}
