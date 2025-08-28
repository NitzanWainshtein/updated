#include "AlgorithmFactory.h"
#include "EulerAlgorithm.h"
#include "Max_clique_algorithm.h"
#include "Mst_algorithm.h"
#include "hamilton_algorithm.h"
#include <stdexcept>
#include <algorithm>

std::unique_ptr<BaseAlgorithm> AlgorithmFactory::createAlgorithm(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::EULER_CIRCUIT:
            return std::make_unique<EulerAlgorithm>();
        case AlgorithmType::MAX_CLIQUE:
            return std::make_unique<MaxCliqueAlgorithm>();
        case AlgorithmType::MST_WEIGHT:
            return std::make_unique<MSTAlgorithm>();
        case AlgorithmType::HAMILTON_CIRCUIT:
            return std::make_unique<HamiltonAlgorithm>();
        default:
            throw std::invalid_argument("Unknown algorithm type");
    }
}

std::unique_ptr<BaseAlgorithm> AlgorithmFactory::createAlgorithm(const std::string& name) {
    return createAlgorithm(stringToType(name));
}

std::vector<std::string> AlgorithmFactory::getAllAlgorithmNames() {
    return {"euler", "clique", "mst", "hamilton"};
}

std::vector<AlgorithmType> AlgorithmFactory::getAllAlgorithmTypes() {
    return {
        AlgorithmType::EULER_CIRCUIT,
        AlgorithmType::MAX_CLIQUE,
        AlgorithmType::MST_WEIGHT,
        AlgorithmType::HAMILTON_CIRCUIT
    };
}

std::string AlgorithmFactory::typeToString(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::EULER_CIRCUIT: return "euler";
        case AlgorithmType::MAX_CLIQUE: return "clique";
        case AlgorithmType::MST_WEIGHT: return "mst";
        case AlgorithmType::HAMILTON_CIRCUIT: return "hamilton";
        default: throw std::invalid_argument("Unknown algorithm type");
    }
}

AlgorithmType AlgorithmFactory::stringToType(const std::string& name) {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    if (lowerName == "euler") return AlgorithmType::EULER_CIRCUIT;
    if (lowerName == "clique") return AlgorithmType::MAX_CLIQUE;
    if (lowerName == "mst") return AlgorithmType::MST_WEIGHT;
    if (lowerName == "hamilton") return AlgorithmType::HAMILTON_CIRCUIT;
    
    throw std::invalid_argument("Unknown algorithm: " + name);
}

bool AlgorithmFactory::isValidAlgorithm(const std::string& name) {
    try {
        stringToType(name);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    }
}
