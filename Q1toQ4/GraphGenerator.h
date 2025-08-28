#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "Graph.h"
#include <random>

class GraphGenerator {
private:
    std::mt19937 generator;
    
public:
    // Constructor with seed
    explicit GraphGenerator(unsigned int seed = std::random_device{}());
    
    // Generate random graph
    Graph generateRandomGraph(int vertices, int edges, bool directed = false);
    
    // Generate specific types of graphs
    Graph generateConnectedGraph(int vertices, int edges, bool directed = false);
    Graph generateCompleteGraph(int vertices, bool directed = false);
    Graph generateCycleGraph(int vertices, bool directed = false);
    Graph generatePathGraph(int vertices, bool directed = false);
    Graph generateStarGraph(int vertices, bool directed = false);
    
    // Generate graph with Euler circuit guarantee
    Graph generateEulerianGraph(int vertices, int edges, bool directed = false);
    
    // Utility methods
    void setSeed(unsigned int seed);
    static int getMaxEdges(int vertices, bool directed);
    static bool isValidEdgeCount(int vertices, int edges, bool directed);
    
private:
    std::pair<int, int> generateRandomEdge(int vertices, const std::vector<std::pair<int, int>>& existing);
    bool hasEdge(const std::vector<std::pair<int, int>>& edges, int u, int v, bool directed);
};

#endif // GRAPH_GENERATOR_H