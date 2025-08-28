#ifndef EULER_ALGORITHM_H
#define EULER_ALGORITHM_H

#include "BaseAlgorithm.h"

class EulerAlgorithm : public BaseAlgorithm {
public:
    AlgorithmResult execute(const Graph& graph) override;
    std::string getName() const override { return "Euler Circuit"; }
    bool supportsDirected() const override { return true; }
    bool supportsUndirected() const override { return true; }
};

#endif
