#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "BaseAlgorithm.h"
#include <string>
#include <vector>
#include <memory>

enum class AlgorithmType {
    EULER_CIRCUIT,
    MAX_CLIQUE,
    MST_WEIGHT,
    HAMILTON_CIRCUIT
};

class AlgorithmFactory {
public:
    static std::unique_ptr<BaseAlgorithm> createAlgorithm(AlgorithmType type);
    static std::unique_ptr<BaseAlgorithm> createAlgorithm(const std::string& name);
    static std::vector<std::string> getAllAlgorithmNames();
    static std::vector<AlgorithmType> getAllAlgorithmTypes();
    static std::string typeToString(AlgorithmType type);
    static AlgorithmType stringToType(const std::string& name);
    static bool isValidAlgorithm(const std::string& name);
};

#endif
