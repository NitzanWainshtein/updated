#include "Mst_algorithm.h"
#include <chrono>
#include <algorithm>
#include <sstream>

AlgorithmResult MSTAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    AlgorithmResult result;
    
    try {
        if (graph.isDirected()) {
            result.success = false;
            result.error = "MST algorithm works only on undirected graphs";
            return result;
        }
        
        if (!graph.isConnected()) {
            result.success = false;
            result.error = "Graph must be connected for MST";
            return result;
        }
        
        int n = graph.getVertexCount();
        if (n <= 1) {
            result.success = true;
            result.result = "Weight: 0 (trivial case)";
            return result;
        }
        
        // Get edges with uniform weights (weight = 1 for simplicity)
        std::vector<Edge> edges = getEdgesWithWeights(graph);
        
        // Sort edges by weight (all are 1, so no change needed)
        std::sort(edges.begin(), edges.end());
        
        // Kruskal's algorithm with Union-Find
        std::vector<int> parent(n);
        std::vector<int> rank(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
        
        int totalWeight = 0;
        int edgesUsed = 0;
        std::vector<Edge> mstEdges;
        
        for (const Edge& edge : edges) {
            int rootU = find(parent, edge.u);
            int rootV = find(parent, edge.v);
            
            if (rootU != rootV) {
                unite(parent, rank, edge.u, edge.v);
                totalWeight += edge.weight;
                mstEdges.push_back(edge);
                edgesUsed++;
                
                if (edgesUsed == n - 1) break; // MST is complete
            }
        }
        
        std::ostringstream output;
        output << "Weight: " << totalWeight;
        output << " (Edges: " << edgesUsed << ")";
        
        if (mstEdges.size() <= 8) {
            output << " - MST edges: ";
            for (size_t i = 0; i < mstEdges.size(); i++) {
                output << mstEdges[i].u << "-" << mstEdges[i].v;
                if (i < mstEdges.size() - 1) output << ",";
            }
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

int MSTAlgorithm::find(std::vector<int>& parent, int x) {
    if (parent[x] != x) {
        parent[x] = find(parent, parent[x]); // Path compression
    }
    return parent[x];
}

void MSTAlgorithm::unite(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);
    
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

std::vector<Edge> MSTAlgorithm::getEdgesWithWeights(const Graph& graph) {
    std::vector<Edge> edges;
    
    // Since our graph doesn't have weights, assign weight 1 to all edges
    for (int u = 0; u < graph.getVertexCount(); u++) {
        std::vector<int> neighbors = graph.getNeighbors(u);
        for (int v : neighbors) {
            if (u < v) { // Avoid duplicates in undirected graph
                edges.emplace_back(u, v, 1);
            }
        }
    }
    
    return edges;
}