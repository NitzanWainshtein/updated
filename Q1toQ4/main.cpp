#include "Graph.h"
#include "GraphGenerator.h"
#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <chrono>
#include <iomanip>

// Function prototypes
void printUsage(const char* programName);
void printGraphInfo(const Graph& graph);
void printEulerResult(const Graph& graph);
void runTests();

int main(int argc, char* argv[]) {
    int vertices = -1;
    int edges = -1;
    unsigned int seed = 0;
    bool directed = false;
    bool runTestMode = false;
    bool verbose = false;
    bool seedProvided = false;
    
    // getopt long options
    static struct option long_options[] = {
        {"vertices",    required_argument, 0, 'v'},
        {"edges",       required_argument, 0, 'e'},
        {"seed",        required_argument, 0, 's'},
        {"directed",    no_argument,       0, 'd'},
        {"test",        no_argument,       0, 't'},
        {"verbose",     no_argument,       0, 'V'},
        {"help",        no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "v:e:s:dtVh", long_options, &option_index)) != -1) {
        switch (c) {
            case 'v':
                vertices = std::atoi(optarg);
                if (vertices <= 0) {
                    std::cerr << "Error: Number of vertices must be positive\n";
                    return 1;
                }
                break;
                
            case 'e':
                edges = std::atoi(optarg);
                if (edges < 0) {
                    std::cerr << "Error: Number of edges must be non-negative\n";
                    return 1;
                }
                break;
                
            case 's':
                seed = static_cast<unsigned int>(std::atol(optarg));
                seedProvided = true;
                break;
                
            case 'd':
                directed = true;
                break;
                
            case 't':
                runTestMode = true;
                break;
                
            case 'V':
                verbose = true;
                break;
                
            case 'h':
                printUsage(argv[0]);
                return 0;
                
            case '?':
                std::cerr << "Use --help for usage information\n";
                return 1;
                
            default:
                break;
        }
    }
    
    // Validate required parameters
    if (!runTestMode) {
        if (vertices == -1 || edges == -1 || !seedProvided) {
            std::cerr << "Error: Missing required parameters\n";
            std::cerr << "Usage: " << argv[0] << " -v <vertices> -e <edges> -s <seed>\n";
            return 1;
        }
    }
    
    try {
        if (runTestMode) {
            std::cout << "Running built-in tests\n\n";
            runTests();
            return 0;
        }
        
        // Validate edge count
        if (!GraphGenerator::isValidEdgeCount(vertices, edges, directed)) {
            int maxEdges = GraphGenerator::getMaxEdges(vertices, directed);
            std::cerr << "Error: Too many edges. Maximum for " << vertices 
                     << " vertices (" << (directed ? "directed" : "undirected") 
                     << ") is " << maxEdges << "\n";
            return 1;
        }
        
        std::cout << "Graph parameters: " << vertices << " vertices, " 
                  << edges << " edges, seed=" << seed 
                  << " (" << (directed ? "directed" : "undirected") << ")\n";
        
        GraphGenerator generator(seed);
        Graph graph = generator.generateRandomGraph(vertices, edges, directed);
        
        printGraphInfo(graph);
        
        if (verbose) {
            std::cout << "\nAdjacency list:\n";
            graph.printGraph();
        }
        
        printEulerResult(graph);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " -v <vertices> -e <edges> -s <seed> [options]\n\n";
    std::cout << "Required:\n";
    std::cout << "  -v, --vertices N    Number of vertices\n";
    std::cout << "  -e, --edges N       Number of edges\n";
    std::cout << "  -s, --seed N        Random seed\n\n";
    std::cout << "Options:\n";
    std::cout << "  -d, --directed      Directed graph\n";
    std::cout << "  -V, --verbose       Print adjacency list\n";
    std::cout << "  -t, --test          Run tests\n";
    std::cout << "  -h, --help          Show help\n";
}

void printGraphInfo(const Graph& graph) {
    std::cout << "Graph: " << graph.getVertexCount() << " vertices, " 
              << graph.getEdgeCount() << " edges, " 
              << (graph.isDirected() ? "directed" : "undirected") 
              << ", " << (graph.isConnected() ? "connected" : "disconnected") << "\n";
}

void printEulerResult(const Graph& graph) {
    std::cout << "\nEuler circuit: ";
    
    bool hasEuler = graph.hasEulerCircuit();
    std::cout << (hasEuler ? "YES" : "NO") << "\n";
    
    if (hasEuler) {
        Graph tempGraph = graph;
        std::vector<int> circuit = tempGraph.findEulerCircuit();
        
        if (!circuit.empty() && circuit.size() <= 20) {
            std::cout << "Circuit: ";
            for (size_t i = 0; i < circuit.size(); ++i) {
                std::cout << circuit[i];
                if (i < circuit.size() - 1) std::cout << " -> ";
            }
            std::cout << "\n";
        }
    }
}

void runTests() {
    GraphGenerator generator(42);
    
    std::cout << "Test 1: Triangle\n";
    Graph triangle = generator.generateCycleGraph(3, false);
    printGraphInfo(triangle);
    printEulerResult(triangle);
    
    std::cout << "\nTest 2: Path graph\n";
    Graph path = generator.generatePathGraph(4, false);
    printGraphInfo(path);
    printEulerResult(path);
    
    std::cout << "\nTest 3: Complete graph K4\n";
    Graph complete = generator.generateCompleteGraph(4, false);
    printGraphInfo(complete);
    printEulerResult(complete);
}