# Q1-Q4: Graph Data Structure & Euler Circuit Analysis

Implementation of OS project requirements 1-4: Graph data structure, Euler circuit algorithm, random graph generation with getopt, and analysis tools.

## Requirements Met

1. **Graph Data Structure** - Adjacency list implementation supporting directed/undirected graphs
2. **Euler Circuit Algorithm** - Detection and circuit finding using Hierholzer's algorithm
3. **Random Graph Generation** - Command-line interface using getopt(3) with mandatory parameters
4. **Analysis Tools** - Code coverage, gprof profiling, and Valgrind memory analysis

## Build and Run

### Compilation
```bash
make
```

### Basic Usage
All parameters are mandatory (same as ex.1):
```bash
./graph_analyzer -v <vertices> -e <edges> -s <seed>
```

### Examples
```bash
# Basic undirected graph
./graph_analyzer -v 4 -e 4 -s 42

# Directed graph with verbose output
./graph_analyzer -v 5 -e 8 -s 123 --directed --verbose

# Built-in tests (bypasses parameter requirement)
./graph_analyzer --test
```

### Command Line Options
- `-v, --vertices N` - Number of vertices (required)
- `-e, --edges N` - Number of edges (required)  
- `-s, --seed N` - Random seed (required)
- `-d, --directed` - Generate directed graph
- `-V, --verbose` - Show adjacency list
- `-t, --test` - Run built-in tests
- `-h, --help` - Show help

## Analysis Tools (Q4)

### Code Coverage
```bash
make coverage
ls *.gcov  # View coverage files
```

### Performance Profiling
```bash
make profile
head gprof_report.txt  # View profiling results
```

### Memory Analysis
```bash
make valgrind
head valgrind_report.txt  # View memory analysis
```

### Run All Analysis
```bash
make analyze
```

## Clean Up
```bash
make clean
```

## Expected Output

**Euler Circuit Detection:**
- Triangle graphs: YES with circuit displayed
- Path graphs: NO
- Complete K4: NO (odd degree vertices)

**Parameter Validation:**
- Missing parameters trigger error message
- All three parameters (-v, -e, -s) are mandatory

**Analysis Reports:**
- Coverage: .gcov files generated
- Profiling: gprof_report.txt created
- Valgrind: valgrind_report.txt with memory analysis

## Files
- `Graph.h/cpp` - Graph data structure implementation
- `GraphGenerator.h/cpp` - Random graph generation
- `main.cpp` - Main program with getopt interface
- `Makefile` - Build system with analysis tools