#include "GraphGenerator.h"
#include <algorithm>
#include <set>
#include <stdexcept>

GraphGenerator::GraphGenerator(unsigned int seed) : generator(seed) {}

void GraphGenerator::setSeed(unsigned int seed) {
    generator.seed(seed);
}

int GraphGenerator::getMaxEdges(int vertices, bool directed) {
    if (vertices <= 1) return 0;
    
    if (directed) {
        return vertices * (vertices - 1); // n(n-1) for directed
    } else {
        return vertices * (vertices - 1) / 2; // n(n-1)/2 for undirected
    }
}

bool GraphGenerator::isValidEdgeCount(int vertices, int edges, bool directed) {
    if (vertices <= 0 || edges < 0) return false;
    return edges <= getMaxEdges(vertices, directed);
}

Graph GraphGenerator::generateRandomGraph(int vertices, int edges, bool directed) {
    if (!isValidEdgeCount(vertices, edges, directed)) {
        throw std::invalid_argument("Invalid edge count for given vertices");
    }
    
    Graph graph(vertices, directed);
    
    if (edges == 0) return graph;
    
    std::vector<std::pair<int, int>> edgeList;
    std::set<std::pair<int, int>> edgeSet;
    
    // Generate all possible edges
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j) { // No self loops
                if (directed) {
                    edgeList.push_back({i, j});
                } else if (i < j) { // For undirected, only add once
                    edgeList.push_back({i, j});
                }
            }
        }
    }
    
    // Shuffle the edge list
    std::shuffle(edgeList.begin(), edgeList.end(), generator);
    
    // Add the first 'edges' edges
    for (int i = 0; i < edges && i < static_cast<int>(edgeList.size()); ++i) {
        graph.addEdge(edgeList[i].first, edgeList[i].second);
    }
    
    return graph;
}

Graph GraphGenerator::generateConnectedGraph(int vertices, int edges, bool directed) {
    if (vertices <= 1) {
        return Graph(vertices, directed);
    }
    
    int minEdges = vertices - 1; // Minimum for connectivity
    if (edges < minEdges) {
        throw std::invalid_argument("Not enough edges to create connected graph");
    }
    
    Graph graph(vertices, directed);
    
    // First create a spanning tree to ensure connectivity
    std::vector<int> vertices_list;
    for (int i = 0; i < vertices; ++i) {
        vertices_list.push_back(i);
    }
    
    std::shuffle(vertices_list.begin(), vertices_list.end(), generator);
    
    // Connect vertices in a random tree
    for (int i = 1; i < vertices; ++i) {
        std::uniform_int_distribution<int> dist(0, i - 1);
        int parent = dist(generator);
        graph.addEdge(vertices_list[parent], vertices_list[i]);
    }
    
    // Add remaining edges randomly
    int remainingEdges = edges - minEdges;
    std::vector<std::pair<int, int>> possibleEdges;
    
    // Generate all possible additional edges
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j && !graph.hasEdge(i, j)) {
                if (directed) {
                    possibleEdges.push_back({i, j});
                } else if (i < j) {
                    possibleEdges.push_back({i, j});
                }
            }
        }
    }
    
    std::shuffle(possibleEdges.begin(), possibleEdges.end(), generator);
    
    for (int i = 0; i < remainingEdges && i < static_cast<int>(possibleEdges.size()); ++i) {
        graph.addEdge(possibleEdges[i].first, possibleEdges[i].second);
    }
    
    return graph;
}

Graph GraphGenerator::generateCompleteGraph(int vertices, bool directed) {
    Graph graph(vertices, directed);
    
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j) {
                graph.addEdge(i, j);
            }
        }
    }
    
    return graph;
}

Graph GraphGenerator::generateCycleGraph(int vertices, bool directed) {
    if (vertices < 3) {
        throw std::invalid_argument("Cycle graph needs at least 3 vertices");
    }
    
    Graph graph(vertices, directed);
    
    for (int i = 0; i < vertices; ++i) {
        graph.addEdge(i, (i + 1) % vertices);
    }
    
    return graph;
}

Graph GraphGenerator::generatePathGraph(int vertices, bool directed) {
    if (vertices <= 1) {
        return Graph(vertices, directed);
    }
    
    Graph graph(vertices, directed);
    
    for (int i = 0; i < vertices - 1; ++i) {
        graph.addEdge(i, i + 1);
    }
    
    return graph;
}

Graph GraphGenerator::generateStarGraph(int vertices, bool directed) {
    if (vertices <= 1) {
        return Graph(vertices, directed);
    }
    
    Graph graph(vertices, directed);
    
    // Connect vertex 0 to all other vertices
    for (int i = 1; i < vertices; ++i) {
        graph.addEdge(0, i);
    }
    
    return graph;
}

Graph GraphGenerator::generateEulerianGraph(int vertices, int edges, bool directed) {
    if (vertices < 2) {
        throw std::invalid_argument("Eulerian graph needs at least 2 vertices");
    }
    
    // Start with a cycle to ensure Eulerian property
    Graph graph = generateCycleGraph(vertices, directed);
    
    int currentEdges = vertices; // Cycle has n edges
    int remainingEdges = edges - currentEdges;
    
    if (remainingEdges < 0) {
        throw std::invalid_argument("Not enough edges to create Eulerian graph");
    }
    
    // Add pairs of edges to maintain even degree (undirected) or in-degree = out-degree (directed)
    std::vector<std::pair<int, int>> possibleEdges;
    
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j && !graph.hasEdge(i, j)) {
                if (directed) {
                    possibleEdges.push_back({i, j});
                } else if (i < j) {
                    possibleEdges.push_back({i, j});
                }
            }
        }
    }
    
    std::shuffle(possibleEdges.begin(), possibleEdges.end(), generator);
    
    if (directed) {
        // For directed graphs, add pairs of edges (i->j and j->i) to maintain balance
        int pairsToAdd = remainingEdges / 2;
        for (int i = 0; i < pairsToAdd && i < static_cast<int>(possibleEdges.size()); ++i) {
            int u = possibleEdges[i].first;
            int v = possibleEdges[i].second;
            if (!graph.hasEdge(u, v) && !graph.hasEdge(v, u)) {
                graph.addEdge(u, v);
                graph.addEdge(v, u);
                currentEdges += 2;
            }
        }
    } else {
        // For undirected graphs, add edges in pairs to maintain even degrees
        std::uniform_int_distribution<int> vertexDist(0, vertices - 1);
        
        while (currentEdges < edges && !possibleEdges.empty()) {
            // Find vertices with odd degree and connect them
            std::vector<int> oddVertices;
            for (int i = 0; i < vertices; ++i) {
                if (graph.getDegree(i) % 2 == 1) {
                    oddVertices.push_back(i);
                }
            }
            
            if (oddVertices.size() >= 2) {
                // Connect two odd-degree vertices
                int u = oddVertices[0];
                int v = oddVertices[1];
                if (!graph.hasEdge(u, v)) {
                    graph.addEdge(u, v);
                    currentEdges++;
                    continue;
                }
            }
            
            // Add a pair of edges to maintain even degrees
            if (possibleEdges.size() >= 2) {
                auto edge1 = possibleEdges.back();
                possibleEdges.pop_back();
                auto edge2 = possibleEdges.back();
                possibleEdges.pop_back();
                
                graph.addEdge(edge1.first, edge1.second);
                graph.addEdge(edge2.first, edge2.second);
                currentEdges += 2;
            } else {
                break;
            }
        }
    }
    
    return graph;
}

std::pair<int, int> GraphGenerator::generateRandomEdge(int vertices, const std::vector<std::pair<int, int>>& existing) {
    std::uniform_int_distribution<int> dist(0, vertices - 1);
    
    int u, v;
    do {
        u = dist(generator);
        v = dist(generator);
    } while (u == v || hasEdge(existing, u, v, false));
    
    return {u, v};
}

bool GraphGenerator::hasEdge(const std::vector<std::pair<int, int>>& edges, int u, int v, bool directed) {
    for (const auto& edge : edges) {
        if ((edge.first == u && edge.second == v) || 
            (!directed && edge.first == v && edge.second == u)) {
            return true;
        }
    }
    return false;
}