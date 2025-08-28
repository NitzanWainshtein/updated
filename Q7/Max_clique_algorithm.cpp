#include "Max_clique_algorithm.h"
#include <chrono>
#include <sstream>

AlgorithmResult MaxCliqueAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    AlgorithmResult result;
    
    try {
        if (graph.isDirected()) {
            result.success = false;
            result.error = "Max clique algorithm works only on undirected graphs";
            return result;
        }
        
        if (graph.getVertexCount() > 10) {
            result.success = false;
            result.error = "Graph too large for brute force max clique (max 10 vertices)";
            return result;
        }
        
        std::vector<int> currentClique;
        std::vector<int> candidates;
        std::vector<int> maxClique;
        
        // Initialize candidates with all vertices
        for (int i = 0; i < graph.getVertexCount(); i++) {
            candidates.push_back(i);
        }
        
        findMaxClique(graph, currentClique, candidates, maxClique);
        
        std::ostringstream output;
        output << "Size: " << maxClique.size();
        
        if (!maxClique.empty() && maxClique.size() <= 8) {
            output << " - Vertices: {";
            for (size_t i = 0; i < maxClique.size(); i++) {
                output << maxClique[i];
                if (i < maxClique.size() - 1) output << ",";
            }
            output << "}";
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

void MaxCliqueAlgorithm::findMaxClique(const Graph& graph, std::vector<int>& currentClique, 
                                     std::vector<int>& candidates, std::vector<int>& maxClique) {
    if (candidates.empty()) {
        if (currentClique.size() > maxClique.size()) {
            maxClique = currentClique;
        }
        return;
    }
    
    // Pruning: if current + candidates can't beat max, return
    if (currentClique.size() + candidates.size() <= maxClique.size()) {
        return;
    }
    
    int vertex = candidates[0];
    candidates.erase(candidates.begin());
    
    // Try including this vertex
    currentClique.push_back(vertex);
    std::vector<int> newCandidates = getCommonNeighbors(graph, std::vector<int>{vertex});
    
    // Keep only candidates that are neighbors of all vertices in current clique
    std::vector<int> validCandidates;
    for (int candidate : candidates) {
        bool isNeighborOfAll = true;
        for (int cliqueVertex : currentClique) {
            if (!graph.hasEdge(candidate, cliqueVertex)) {
                isNeighborOfAll = false;
                break;
            }
        }
        if (isNeighborOfAll) {
            validCandidates.push_back(candidate);
        }
    }
    
    findMaxClique(graph, currentClique, validCandidates, maxClique);
    
    // Backtrack
    currentClique.pop_back();
    
    // Try not including this vertex
    findMaxClique(graph, currentClique, candidates, maxClique);
}

std::vector<int> MaxCliqueAlgorithm::getCommonNeighbors(const Graph& graph, const std::vector<int>& vertices) {
    std::vector<int> commonNeighbors;
    
    if (vertices.empty()) return commonNeighbors;
    
    // Get neighbors of first vertex
    std::vector<int> firstNeighbors = graph.getNeighbors(vertices[0]);
    
    // Check which neighbors are common to all vertices
    for (int neighbor : firstNeighbors) {
        bool isCommon = true;
        for (size_t i = 1; i < vertices.size(); i++) {
            if (!graph.hasEdge(vertices[i], neighbor)) {
                isCommon = false;
                break;
            }
        }
        if (isCommon) {
            commonNeighbors.push_back(neighbor);
        }
    }
    
    return commonNeighbors;
}