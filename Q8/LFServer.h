#ifndef LF_SERVER_H
#define LF_SERVER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <atomic>
#include "Graph.h"
#include "AlgorithmFactory.h"

struct ClientRequest {
    int clientSocket;
    int clientId;
    std::string graphData;
    
    ClientRequest(int socket, int id, const std::string& data) 
        : clientSocket(socket), clientId(id), graphData(data) {}
};

struct AllAlgorithmsResult {
    AlgorithmResult euler;
    AlgorithmResult clique;
    AlgorithmResult mst;
    AlgorithmResult hamilton;
    int clientId;
    long totalTime;
    
    // Constructor to initialize properly
    AllAlgorithmsResult(int id = 0) : clientId(id), totalTime(0) {}
};

class LFServer {
private:
    int port;
    int serverSocket;
    std::atomic<bool> running;
    
    // Leader-Follower pattern components
    std::mutex leaderMutex;
    std::condition_variable leaderCondition;
    std::queue<std::shared_ptr<ClientRequest>> requestQueue;
    std::vector<std::thread> workerThreads;
    int numWorkers;
    std::atomic<int> currentLeader;
    std::atomic<int> totalConnections;
    std::atomic<int> totalRequests;
    
    // Statistics
    std::mutex statsMutex;
    int leaderPromotions;
    
public:
    LFServer(int serverPort = 8082, int workers = 4);
    ~LFServer();
    
    void start();
    void stop();
    bool isRunning() const { return running; }
    
private:
    void acceptorThread();
    void workerThread(int workerId);
    void promoteNewLeader();
    Graph parseGraph(const std::string& input);
    AllAlgorithmsResult executeAllAlgorithms(const Graph& graph, int clientId);
    std::string formatAllResults(const AllAlgorithmsResult& results, const Graph& graph);
    void sendResponse(int clientSocket, const std::string& response);
    void logRequest(int clientId, const std::string& message);
};

#endif
