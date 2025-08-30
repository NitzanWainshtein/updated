#ifndef PIPELINE_SERVER_H
#define PIPELINE_SERVER_H

#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include <chrono>
#include "ActiveObject.h"
#include "Graph.h"
#include "AlgorithmFactory.h"
#include "BaseAlgorithm.h"

struct PipelineRequest {
    int clientSocket;
    int clientId;
    std::string inputData;
    std::chrono::high_resolution_clock::time_point startTime;
    
    PipelineRequest(int socket, int id, const std::string& data)
        : clientSocket(socket), clientId(id), inputData(data),
          startTime(std::chrono::high_resolution_clock::now()) {}
};

struct ProcessedGraph {
    int clientSocket;
    int clientId;
    Graph graph;
    std::chrono::high_resolution_clock::time_point startTime;
    
    // Default constructor
    ProcessedGraph() : clientSocket(-1), clientId(-1), graph(1, false) {}
    
    // Constructor with parameters
    ProcessedGraph(int socket, int id, const Graph& g, 
                   const std::chrono::high_resolution_clock::time_point& time)
        : clientSocket(socket), clientId(id), graph(g), startTime(time) {}
};

struct AlgorithmResults {
    int clientSocket;
    int clientId;
    Graph graph;
    AlgorithmResult euler;
    AlgorithmResult clique;
    AlgorithmResult mst;
    AlgorithmResult hamilton;
    std::chrono::high_resolution_clock::time_point startTime;
    
    // Default constructor
    AlgorithmResults() : clientSocket(-1), clientId(-1), graph(1, false) {}
    
    // Constructor with parameters
    AlgorithmResults(int socket, int id, const Graph& g,
                     const std::chrono::high_resolution_clock::time_point& time)
        : clientSocket(socket), clientId(id), graph(g), startTime(time) {}
};

class PipelineServer {
private:
    int port;
    int serverSocket;
    std::atomic<bool> running;
    
    // 4 Active Objects for pipeline stages
    std::unique_ptr<ActiveObject> requestReceiver;   // Stage 1: Receive and parse
    std::unique_ptr<ActiveObject> graphProcessor;    // Stage 2: Validate and prepare graph
    std::unique_ptr<ActiveObject> algorithmExecutor; // Stage 3: Execute all 4 algorithms
    std::unique_ptr<ActiveObject> responseFormatter; // Stage 4: Format and send response
    
    // Statistics
    std::atomic<int> totalConnections;
    std::atomic<int> totalRequests;
    std::atomic<int> pipelineErrors;
    std::mutex statsMutex;
    
public:
    PipelineServer(int serverPort = 8083);
    ~PipelineServer();
    
    void start();
    void stop();
    bool isRunning() const { return running.load(); }
    
    // Pipeline statistics
    struct PipelineStats {
        int totalRequests;
        int totalErrors;
        int stage1Processed;
        int stage2Processed;
        int stage3Processed;
        int stage4Processed;
        int stage1Queue;
        int stage2Queue;
        int stage3Queue;
        int stage4Queue;
    };
    
    PipelineStats getStats();
    
private:
    void acceptorThread();
    void handleClientConnection(int clientSocket, int clientId);
    
    // Pipeline stages
    void stage1_receiveRequest(std::shared_ptr<PipelineRequest> request);
    void stage2_processGraph(std::shared_ptr<ProcessedGraph> graph);
    void stage3_executeAlgorithms(std::shared_ptr<AlgorithmResults> results);
    void stage4_formatResponse(std::shared_ptr<AlgorithmResults> results);
    
    // Utility methods
    Graph parseGraphInput(const std::string& input);
    std::string formatPipelineResponse(const AlgorithmResults& results);
    void sendResponse(int clientSocket, const std::string& response);
    void logRequest(int clientId, const std::string& message);
    std::string formatError(const std::string& error, int clientId);
};

#endif