#ifndef MST_ALGORITHM_H
#define MST_ALGORITHM_H

#include "BaseAlgorithm.h"
#include <vector>

struct Edge {
    int u, v, weight;
    Edge(int u, int v, int w) : u(u), v(v), weight(w) {}
    bool operator<(const Edge& other) const { return weight < other.weight; }
};

class MSTAlgorithm : public BaseAlgorithm {
public:
    AlgorithmResult execute(const Graph& graph) override;
    std::string getName() const override { return "Minimum Spanning Tree Weight"; }
    bool supportsDirected() const override { return false; }
    bool supportsUndirected() const override { return true; }

private:
    int find(std::vector<int>& parent, int x);
    void unite(std::vector<int>& parent, std::vector<int>& rank, int x, int y);
    std::vector<Edge> getEdgesWithWeights(const Graph& graph);
};

#endif