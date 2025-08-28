# Q7: Multi-Algorithms Server (Strategy/Factory Patterns)

Server implementing 4 graph algorithms using Strategy and Factory design patterns as required by the assignment.

## Prerequisites

```bash
cd ../Q1toQ4
make
cd ../Q7
```

## Build

```bash
make clean
make
```

## Run

### Server
```bash
./algorithms_server [port]
# Default port: 8081
```

### Client
```bash
./algorithms_client [host] [port]
# Default: localhost 8081
```

### Alternative Connection
```bash
telnet localhost 8081
```

## Four Algorithms Implemented (Q7 Requirement)

### 1. Euler Circuit
Finds Euler circuit or proves none exists (works on both directed/undirected).

**Command:**
```bash
euler vertices edges edge1 edge2 ...
```

**Examples:**
```bash
# Triangle - has Euler circuit
euler 3 3 0-1 1-2 2-0
# Result: YES - Circuit: 0->1->2->0

# Path - no Euler circuit  
euler 4 3 0-1 1-2 2-3
# Result: NO
```

### 2. Maximum Clique
Finds maximum clique in undirected graphs (brute force algorithm).

**Command:**
```bash
clique vertices edges edge1 edge2 ...
```

**Examples:**
```bash
# Complete graph K4 - clique of size 4
clique 4 6 0-1 0-2 0-3 1-2 1-3 2-3
# Result: Size: 4 - Vertices: {0,1,2,3}

# Triangle - clique of size 3
clique 3 3 0-1 1-2 2-0
# Result: Size: 3 - Vertices: {0,1,2}
```

### 3. Minimum Spanning Tree (MST)
Calculates MST weight using Kruskal's algorithm on undirected graphs.

**Command:**
```bash
mst vertices edges edge1 edge2 ...
```

**Examples:**
```bash
# Square graph - MST needs 3 edges
mst 4 4 0-1 1-2 2-3 3-0
# Result: Weight: 3 (Edges: 3) - MST edges: 0-1,0-3,1-2

# Connected graph
mst 5 6 0-1 0-2 1-3 2-3 3-4 1-4
# Result: Weight: 4 (Edges: 4)
```

### 4. Hamilton Circuit
Finds Hamilton circuit using backtracking (works on any graph).

**Command:**
```bash
hamilton vertices edges edge1 edge2 ...
```

**Examples:**
```bash
# Square with cycle - has Hamilton circuit
hamilton 4 4 0-1 1-2 2-3 3-0
# Result: YES - Circuit: 0->1->2->3->0

# Path graph - no Hamilton circuit
hamilton 4 3 0-1 1-2 2-3
# Result: NO
```

## Design Patterns Implementation

### Strategy Pattern
Each algorithm implements the `BaseAlgorithm` interface:

```cpp
class BaseAlgorithm {
public:
    virtual AlgorithmResult execute(const Graph& graph) = 0;
    virtual std::string getName() const = 0;
    virtual bool supportsDirected() const = 0;
    virtual bool supportsUndirected() const = 0;
};
```

Concrete strategies:
- `EulerAlgorithm` - Hierholzer's algorithm
- `MaxCliqueAlgorithm` - Brute force with pruning
- `MSTAlgorithm` - Kruskal's with Union-Find
- `HamiltonAlgorithm` - Backtracking search

### Factory Pattern
`AlgorithmFactory` creates algorithms dynamically:

```cpp
// Create by string name
auto algorithm = AlgorithmFactory::createAlgorithm("euler");

// Validate algorithm names
bool valid = AlgorithmFactory::isValidAlgorithm("mst");

// List all available algorithms
auto names = AlgorithmFactory::getAllAlgorithmNames();
```

## Server Commands

- `help` - Show format and available algorithms
- `quit` - Exit connection
- Invalid algorithm names return error with available options

## Expected Results Summary

| Algorithm | Graph Type | Input Example | Expected Output |
|-----------|------------|---------------|-----------------|
| euler | Triangle | `3 3 0-1 1-2 2-0` | `YES - Circuit: 0->1->2->0` |
| clique | Complete K4 | `4 6 0-1 0-2 0-3 1-2 1-3 2-3` | `Size: 4 - Vertices: {0,1,2,3}` |
| mst | Square | `4 4 0-1 1-2 2-3 3-0` | `Weight: 3 (Edges: 3)` |
| hamilton | Square | `4 4 0-1 1-2 2-3 3-0` | `YES - Circuit: 0->1->2->3->0` |

## Performance Notes

- All algorithms include execution time measurement
- Max Clique and Hamilton use brute force (suitable for small graphs)
- MST uses efficient Kruskal's algorithm with Union-Find
- Euler uses linear-time Hierholzer's algorithm

## Files

- `server_main.cpp` - Multi-threaded TCP server implementation
- `client.cpp` - Simple client for testing
- `BaseAlgorithm.h` - Strategy pattern interface
- `AlgorithmFactory.h/cpp` - Factory pattern implementation
- `EulerAlgorithm.h/cpp` - Euler circuit strategy
- `Max_clique_algorithm.h/cpp` - Maximum clique strategy
- `Mst_algorithm.h/cpp` - MST weight strategy  
- `hamilton_algorithm.h/cpp` - Hamilton circuit strategy
- `Makefile` - Build system (uses Graph from ../Q1toQ4)