#ifndef BASE_ALGORITHM_H
#define BASE_ALGORITHM_H

#include "Graph.h"
#include <string>
#include <memory>

struct AlgorithmResult {
    bool success;
    std::string result;
    std::string error;
    long executionTime;
    
    AlgorithmResult() : success(false), executionTime(0) {}
};

class BaseAlgorithm {
public:
    virtual ~BaseAlgorithm() = default;
    virtual AlgorithmResult execute(const Graph& graph) = 0;
    virtual std::string getName() const = 0;
    virtual bool supportsDirected() const = 0;
    virtual bool supportsUndirected() const = 0;
};

#endif
