#ifndef MAX_CLIQUE_ALGORITHM_H
#define MAX_CLIQUE_ALGORITHM_H

#include "BaseAlgorithm.h"
#include <vector>

class MaxCliqueAlgorithm : public BaseAlgorithm {
public:
    AlgorithmResult execute(const Graph& graph) override;
    std::string getName() const override { return "Maximum Clique"; }
    bool supportsDirected() const override { return false; }
    bool supportsUndirected() const override { return true; }

private:
    void findMaxClique(const Graph& graph, std::vector<int>& currentClique, 
                      std::vector<int>& candidates, std::vector<int>& maxClique);
    std::vector<int> getCommonNeighbors(const Graph& graph, const std::vector<int>& vertices);
};

#endif