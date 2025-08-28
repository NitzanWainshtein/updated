#ifndef HAMILTON_ALGORITHM_H
#define HAMILTON_ALGORITHM_H

#include "BaseAlgorithm.h"
#include <vector>

class HamiltonAlgorithm : public BaseAlgorithm {
public:
    AlgorithmResult execute(const Graph& graph) override;
    std::string getName() const override { return "Hamilton Circuit"; }
    bool supportsDirected() const override { return true; }
    bool supportsUndirected() const override { return true; }

private:
    bool findHamiltonCircuit(const Graph& graph, std::vector<int>& path, std::vector<bool>& visited, int pos);
    bool isValidNext(const Graph& graph, int v, const std::vector<int>& path, int pos);
};

#endif