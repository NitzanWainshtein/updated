#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <iostream>
#include <stdexcept>
#include <algorithm>

class Graph {
private:
    int vertices;
    bool directed;
    std::vector<std::list<int>> adjList;
    
public:
    // Constructors
    Graph(int v, bool isDirected = false);
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    ~Graph() = default;
    
    // Basic operations
    void addEdge(int u, int v);
    void removeEdge(int u, int v);
    std::vector<int> getNeighbors(int vertex) const;
    
    // Getters
    int getVertexCount() const { return vertices; }
    bool isDirected() const { return directed; }
    
    // Display
    void printGraph() const;
    std::string getGraphString() const;
    
    // Validation
    bool isValidVertex(int vertex) const;
    bool hasEdge(int u, int v) const;
    int getDegree(int vertex) const;
    int getInDegree(int vertex) const;
    int getOutDegree(int vertex) const;
    
    // Graph properties
    bool isEmpty() const;
    int getEdgeCount() const;
    bool isConnected() const;
    
    // Euler circuit methods
    bool hasEulerCircuit() const;
    std::vector<int> findEulerCircuit();
    
    // Utility methods
    void clear();
    std::vector<std::pair<int, int>> getAllEdges() const;
    Graph getUndirectedCopy() const;
    
private:
    void dfsUtil(int v, std::vector<bool>& visited) const;
    void hierholzerAlgorithm(std::vector<int>& circuit);
    bool isEulerianConnected() const;
};

#endif // GRAPH_H