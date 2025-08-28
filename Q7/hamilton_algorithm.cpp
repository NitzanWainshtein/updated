#include "hamilton_algorithm.h"
#include <chrono>
#include <sstream>

AlgorithmResult HamiltonAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    AlgorithmResult result;
    
    try {
        int n = graph.getVertexCount();
        
        if (n == 0) {
            result.success = true;
            result.result = "YES (trivial case)";
            return result;
        }
        
        if (n > 10) {
            result.success = false;
            result.error = "Graph too large for brute force Hamilton circuit (max 10 vertices)";
            return result;
        }
        
        std::vector<int> path(n);
        std::vector<bool> visited(n, false);
        
        // Try starting from vertex 0
        path[0] = 0;
        visited[0] = true;
        
        bool foundCircuit = findHamiltonCircuit(graph, path, visited, 1);
        
        std::ostringstream output;
        output << (foundCircuit ? "YES" : "NO");
        
        if (foundCircuit && n <= 8) {
            output << " - Circuit: ";
            for (int i = 0; i < n; i++) {
                output << path[i];
                if (i < n - 1) output << "->";
            }
            output << "->" << path[0]; // Complete the circuit
        } else if (foundCircuit) {
            output << " - Circuit found (length: " << n << ")";
        }
        
        result.success = true;
        result.result = output.str();
        
    } catch (const std::exception& e) {
        result.success = false;
        result.error = e.what();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    return result;
}

bool HamiltonAlgorithm::findHamiltonCircuit(const Graph& graph, std::vector<int>& path, 
                                           std::vector<bool>& visited, int pos) {
    int n = graph.getVertexCount();
    
    if (pos == n) {
        // All vertices visited, check if we can return to start
        return graph.hasEdge(path[pos - 1], path[0]);
    }
    
    for (int v = 1; v < n; v++) { // Start from 1 since 0 is already in path
        if (isValidNext(graph, v, path, pos) && !visited[v]) {
            path[pos] = v;
            visited[v] = true;
            
            if (findHamiltonCircuit(graph, path, visited, pos + 1)) {
                return true;
            }
            
            // Backtrack
            visited[v] = false;
        }
    }
    
    return false;
}

bool HamiltonAlgorithm::isValidNext(const Graph& graph, int v, const std::vector<int>& path, int pos) {
    // Check if vertex v can be added at position pos
    if (pos == 0) return true;
    
    // Check if there's an edge from previous vertex to v
    return graph.hasEdge(path[pos - 1], v);
}