#include "EulerAlgorithm.h"
#include <chrono>
#include <sstream>

AlgorithmResult EulerAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    AlgorithmResult result;
    
    try {
        bool hasEuler = graph.hasEulerCircuit();
        
        std::ostringstream output;
        output << (hasEuler ? "YES" : "NO");
        
        if (hasEuler) {
            Graph tempGraph = graph;
            std::vector<int> circuit = tempGraph.findEulerCircuit();
            
            if (!circuit.empty() && circuit.size() <= 15) {
                output << " - Circuit: ";
                for (size_t i = 0; i < circuit.size(); i++) {
                    output << circuit[i];
                    if (i < circuit.size() - 1) output << "->";
                }
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
