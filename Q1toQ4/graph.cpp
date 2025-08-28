#include "Graph.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>

// Constructor
Graph::Graph(int v, bool isDirected) : vertices(v), directed(isDirected) {
    if (v <= 0) {
        throw std::invalid_argument("Number of vertices must be positive");
    }
    adjList.resize(vertices);
}

// Copy constructor
Graph::Graph(const Graph& other) : vertices(other.vertices), directed(other.directed) {
    adjList = other.adjList;
}

// Assignment operator
Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        vertices = other.vertices;
        directed = other.directed;
        adjList = other.adjList;
    }
    return *this;
}

// Add edge
void Graph::addEdge(int u, int v) {
    if (!isValidVertex(u) || !isValidVertex(v)) {
        throw std::invalid_argument("Invalid vertex index");
    }
    
    // Check if edge already exists
    if (hasEdge(u, v)) {
        return; // Edge already exists
    }
    
    adjList[u].push_back(v);
    
    if (!directed && u != v) { // Avoid self-loop duplication in undirected graphs
        adjList[v].push_back(u);
    }
}

// Remove edge
void Graph::removeEdge(int u, int v) {
    if (!isValidVertex(u) || !isValidVertex(v)) {
        return;
    }
    
    adjList[u].remove(v);
    
    if (!directed) {
        adjList[v].remove(u);
    }
}

// Get neighbors
std::vector<int> Graph::getNeighbors(int vertex) const {
    if (!isValidVertex(vertex)) {
        throw std::invalid_argument("Invalid vertex index");
    }
    
    std::vector<int> neighbors;
    for (int neighbor : adjList[vertex]) {
        neighbors.push_back(neighbor);
    }
    return neighbors;
}

// Print graph
void Graph::printGraph() const {
    std::cout << "Graph (" << (directed ? "directed" : "undirected") 
              << ", " << vertices << " vertices):\n";
    
    for (int i = 0; i < vertices; ++i) {
        std::cout << i << ": ";
        for (int neighbor : adjList[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << "\n";
    }
}

// Get graph as string
std::string Graph::getGraphString() const {
    std::ostringstream oss;
    oss << "Graph (" << (directed ? "directed" : "undirected") 
        << ", " << vertices << " vertices):\n";
    
    for (int i = 0; i < vertices; ++i) {
        oss << i << ": ";
        for (int neighbor : adjList[i]) {
            oss << neighbor << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

// Validation methods
bool Graph::isValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertices;
}

bool Graph::hasEdge(int u, int v) const {
    if (!isValidVertex(u) || !isValidVertex(v)) {
        return false;
    }
    
    return std::find(adjList[u].begin(), adjList[u].end(), v) != adjList[u].end();
}

int Graph::getDegree(int vertex) const {
    if (!isValidVertex(vertex)) {
        return 0;
    }
    
    if (directed) {
        return getInDegree(vertex) + getOutDegree(vertex);
    } else {
        return adjList[vertex].size();
    }
}

int Graph::getInDegree(int vertex) const {
    if (!isValidVertex(vertex)) {
        return 0;
    }
    
    int inDegree = 0;
    for (int i = 0; i < vertices; ++i) {
        if (hasEdge(i, vertex)) {
            inDegree++;
        }
    }
    return inDegree;
}

int Graph::getOutDegree(int vertex) const {
    if (!isValidVertex(vertex)) {
        return 0;
    }
    
    return adjList[vertex].size();
}

// Graph properties
bool Graph::isEmpty() const {
    return getEdgeCount() == 0;
}

int Graph::getEdgeCount() const {
    int count = 0;
    for (int i = 0; i < vertices; ++i) {
        count += adjList[i].size();
    }
    
    // In undirected graphs, each edge is counted twice
    return directed ? count : count / 2;
}

bool Graph::isConnected() const {
    if (vertices == 0) return true;
    if (vertices == 1) return true;
    
    std::vector<bool> visited(vertices, false);
    
    // Find first vertex with non-zero degree
    int start = 0;
    for (int i = 0; i < vertices; ++i) {
        if (adjList[i].size() > 0) {
            start = i;
            break;
        }
    }
    
    // DFS from start vertex
    dfsUtil(start, visited);
    
    // Check if all vertices with edges are visited
    for (int i = 0; i < vertices; ++i) {
        if (adjList[i].size() > 0 && !visited[i]) {
            return false;
        }
    }
    
    return true;
}

// DFS utility
void Graph::dfsUtil(int v, std::vector<bool>& visited) const {
    visited[v] = true;
    
    for (int neighbor : adjList[v]) {
        if (!visited[neighbor]) {
            dfsUtil(neighbor, visited);
        }
    }
}

// Euler circuit methods
bool Graph::hasEulerCircuit() const {
    if (!isEulerianConnected()) {
        return false;
    }
    
    if (directed) {
        // For directed graphs: in-degree = out-degree for all vertices
        for (int i = 0; i < vertices; ++i) {
            if (getInDegree(i) != getOutDegree(i)) {
                return false;
            }
        }
    } else {
        // For undirected graphs: all vertices must have even degree
        for (int i = 0; i < vertices; ++i) {
            if (adjList[i].size() % 2 != 0) {
                return false;
            }
        }
    }
    
    return true;
}

bool Graph::isEulerianConnected() const {
    if (vertices == 0) return false;
    
    // Create a copy of the graph to check connectivity
    // only considering vertices with non-zero degree
    std::vector<bool> hasEdges(vertices, false);
    for (int i = 0; i < vertices; ++i) {
        if (adjList[i].size() > 0) {
            hasEdges[i] = true;
        }
    }
    
    // Find first vertex with edges
    int start = -1;
    for (int i = 0; i < vertices; ++i) {
        if (hasEdges[i]) {
            start = i;
            break;
        }
    }
    
    if (start == -1) return false; // No edges
    
    // Check if all vertices with edges are reachable
    std::vector<bool> visited(vertices, false);
    dfsUtil(start, visited);
    
    for (int i = 0; i < vertices; ++i) {
        if (hasEdges[i] && !visited[i]) {
            return false;
        }
    }
    
    return true;
}

std::vector<int> Graph::findEulerCircuit() {
    std::vector<int> circuit;
    
    if (!hasEulerCircuit()) {
        return circuit; // Empty circuit if no Euler circuit exists
    }
    
    // Create a copy of the graph for Hierholzer's algorithm
    Graph tempGraph = *this;
    
    // Find starting vertex (any vertex with edges)
    int start = 0;
    for (int i = 0; i < vertices; ++i) {
        if (!tempGraph.adjList[i].empty()) {
            start = i;
            break;
        }
    }
    
    tempGraph.hierholzerAlgorithm(circuit);
    
    return circuit;
}

void Graph::hierholzerAlgorithm(std::vector<int>& circuit) {
    std::stack<int> stack;
    std::vector<int> path;
    
    // Find starting vertex
    int start = 0;
    for (int i = 0; i < vertices; ++i) {
        if (!adjList[i].empty()) {
            start = i;
            break;
        }
    }
    
    stack.push(start);
    
    while (!stack.empty()) {
        int current = stack.top();
        
        if (!adjList[current].empty()) {
            int next = adjList[current].front();
            stack.push(next);
            
            // Remove edge
            removeEdge(current, next);
        } else {
            path.push_back(current);
            stack.pop();
        }
    }
    
    // Reverse to get correct order
    std::reverse(path.begin(), path.end());
    circuit = path;
}

// Utility methods
void Graph::clear() {
    for (auto& list : adjList) {
        list.clear();
    }
}

std::vector<std::pair<int, int>> Graph::getAllEdges() const {
    std::vector<std::pair<int, int>> edges;
    
    for (int i = 0; i < vertices; ++i) {
        for (int neighbor : adjList[i]) {
            if (directed || i <= neighbor) { // Avoid duplicates in undirected graphs
                edges.push_back({i, neighbor});
            }
        }
    }
    
    return edges;
}

Graph Graph::getUndirectedCopy() const {
    if (!directed) {
        return *this; // Already undirected
    }
    
    Graph undirectedGraph(vertices, false);
    
    for (int i = 0; i < vertices; ++i) {
        for (int neighbor : adjList[i]) {
            undirectedGraph.addEdge(i, neighbor);
        }
    }
    
    return undirectedGraph;
}