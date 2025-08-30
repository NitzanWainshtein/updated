# Q10-Q11: Valgrind Analysis & Code Coverage

Comprehensive analysis system for OS project requirements Q10-Q11.

## Requirements

### Q10: Valgrind Analysis
- **Memcheck**: Memory leak detection and invalid memory access
- **Helgrind**: Threading issues and data race detection
- **Callgrind**: CPU profiling and performance analysis

### Q11: Code Coverage
- **Coverage proof**: Complete code coverage analysis
- **Line coverage**: Shows executed code lines across all components

## Usage

### Complete Analysis
```bash
make all
```

### Individual Components
```bash
make memcheck   # Memory analysis
make helgrind   # Threading analysis  
make callgrind  # Performance profiling
make coverage   # Code coverage
```

### Verification
```bash
make verify     # Check requirements status
make results    # View analysis summary
```

## Output Structure

```
reports/
├── valgrind/
│   ├── q1_memcheck.txt         # Q1-Q4 memory analysis
│   ├── q6_server_memcheck.txt  # Q6 server analysis
│   ├── q7_server_memcheck.txt  # Q7 server analysis
│   ├── q8_server_memcheck.txt  # Q8 server analysis
│   ├── q9_server_memcheck.txt  # Q9 server analysis
│   ├── q8_helgrind.txt         # Q8 threading analysis
│   ├── q9_helgrind.txt         # Q9 threading analysis
│   ├── callgrind.q7.out        # Q7 performance data
│   └── callgrind.q9.out        # Q9 performance data
└── coverage/
    ├── coverage_summary.txt    # Coverage summary
    └── *.gcov                  # Individual coverage files
```

## Components Analyzed

- **Q1-Q4**: Graph analyzer with various test scenarios
- **Q6**: Euler circuit server with client interactions
- **Q7**: Multi-algorithm server with Strategy/Factory patterns
- **Q8**: Leader-Follower server with thread management
- **Q9**: Pipeline server with Active Objects

## Expected Results

**Q10 Requirements:**
- Memcheck: 6 reports (all components)
- Helgrind: 2 reports (Q8, Q9 threading)
- Callgrind: 2 profiles (Q7, Q9 performance)

**Q11 Requirements:**
- Coverage files: 70+ .gcov files
- Complete code execution proof across all projects

## Clean Up

```bash
make clean      # Remove all reports and artifacts
```