# OS Final Project

**Authors:** Nitzan Wainshtein (ID: 209086263), Aviv Oz (ID: 207543927)

Complete implementation of OS project requirements covering graph algorithms, design patterns, client-server architecture, threading patterns, and comprehensive analysis tools.

## Project Structure

```
├── Q1toQ4/          # Graph Data Structure & Euler Circuit
├── Q6/              # Euler Circuit Server
├── Q7/              # Multi-Algorithm Server (Strategy/Factory Patterns)
├── Q8/              # LF Threading Pattern
├── Q9/              # Pipeline Pattern with Active Objects
├── Q10toQ11/        # Valgrind Analysis & Code Coverage
├── Makefile         # Main project build system
└── README.md        # This file
```

## Requirements Implemented

### Q1-Q4: Graph Foundation
- **Graph Data Structure**: Adjacency list implementation (directed/undirected)
- **Euler Circuit Algorithm**: Detection and path finding using Hierholzer's algorithm
- **Random Graph Generation**: Command-line interface with getopt
- **Analysis Tools**: Code coverage, gprof, Valgrind reports

### Q6: Network Server
- **Euler Circuit Server**: Multi-threaded TCP server for graph analysis
- **Client Interface**: Command-line client for server communication

### Q7: Algorithm Server with Design Patterns
- **Four Graph Algorithms**: Euler Circuit, Max Clique, MST Weight, Hamilton Circuit
- **Strategy Pattern**: Pluggable algorithm interface
- **Factory Pattern**: Dynamic algorithm creation
- **Multi-threaded Server**: Handles concurrent client requests

### Q8: Leader-Follower Threading
- **Leader-Follower Pattern**: Only one leader accepts requests at a time
- **Thread Management**: Dynamic leader promotion and worker coordination
- **All Algorithms**: Executes all 4 algorithms on each graph request

### Q9: Pipeline Architecture
- **4-Stage Pipeline**: Request → Graph → Algorithms → Response
- **Active Objects**: Each stage runs in dedicated thread with task queue
- **Asynchronous Processing**: True pipeline with overlapping execution

### Q10-Q11: Analysis & Validation
- **Valgrind Analysis**: Memcheck, Helgrind, Callgrind reports
- **Code Coverage**: Comprehensive coverage proof with gcov

## Build & Run

### Complete Build
```bash
make all
```

### Individual Components
```bash
make build-q1-q4    # Graph analyzer
make build-q6       # Euler server  
make build-q7       # Algorithm server
make build-q8       # Leader-Follower server
make build-q9       # Pipeline server
```

### Testing
```bash
# Q1-Q4 automated tests
make test-q1-q4

# Server testing (manual)
# Terminal 1: cd Q6 && ./euler_server
# Terminal 2: cd Q6 && ./euler_client
```

### Analysis (Q10-Q11)
```bash
make analysis       # Complete analysis
make verify         # Check requirements
make results        # View summary
```

## Key Features

**Graph Algorithms:**
- Euler Circuit detection and path construction
- Maximum Clique finding (brute force with pruning)
- Minimum Spanning Tree weight calculation (Kruskal's algorithm)
- Hamilton Circuit detection (backtracking)

**Design Patterns:**
- Strategy pattern for algorithm selection
- Factory pattern for algorithm instantiation
- Active Object pattern for pipeline stages
- Leader-Follower pattern for thread coordination

**Network Architecture:**
- Multi-threaded TCP servers
- Client-server communication protocols
- Graceful connection handling and cleanup

**Analysis & Quality:**
- Memory leak detection with Valgrind
- Threading issue detection with Helgrind
- Performance profiling with Callgrind
- Code coverage verification with gcov

## Usage Examples

### Q1-Q4: Graph Analyzer
```bash
cd Q1toQ4
./graph_analyzer -v 5 -e 7 -s 42          # Generate random graph
./graph_analyzer --test                    # Run built-in tests
```

### Q7: Algorithm Server
```bash
# Server
cd Q7 && ./algorithms_server

# Client
cd Q7 && ./algorithms_client
# Commands: euler 3 3 0-1 1-2 2-0
#          clique 4 6 0-1 0-2 0-3 1-2 1-3 2-3
```

### Q8-Q9: Threading Servers
```bash
# Q8 Leader-Follower
cd Q8 && ./lf_server

# Q9 Pipeline  
cd Q9 && ./pipeline_server
```

## Analysis Results

**Q10: Valgrind Analysis**
- Memcheck: 6 comprehensive memory reports
- Helgrind: 2 threading analysis reports
- Callgrind: 2 performance profiles

**Q11: Code Coverage**
- 70+ .gcov files generated
- Complete code execution proof
- Coverage across all project components

## Requirements Verification

All project requirements have been implemented and verified:
- ✅ Graph data structure and algorithms
- ✅ Design patterns (Strategy, Factory, Active Object, Leader-Follower)
- ✅ Client-server architecture
- ✅ Multi-threading implementations
- ✅ Comprehensive analysis and validation

Run `make verify` for automated requirements checking.

## Clean Up

```bash
make clean          # Clean all components
```