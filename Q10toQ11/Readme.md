# Q10–Q11: Valgrind & Coverage Analysis System

This directory provides a comprehensive analysis system that automatically runs **Valgrind** analyses and **code coverage** reports across the entire OS project (Q1-Q9).

## 🎯 Requirements Coverage

### Q10: Valgrind Analysis
- **Memcheck**: Memory leak detection and invalid memory access
- **Helgrind**: Threading issues and data race detection  
- **Callgrind**: CPU profiling and performance analysis

### Q11: Code Coverage
- **Coverage proof**: Complete code coverage analysis with HTML reports
- **Line coverage**: Shows which lines of code were executed
- **Function coverage**: Shows which functions were called

## 🚀 Quick Start

### Run Complete Analysis (Q10 + Q11)
```bash
cd Q10toQ11
make all
```

This will:
1. Build all projects (Q1-Q9)
2. Run Valgrind memcheck on all components
3. Run Helgrind on multithreaded components (Q8, Q9)
4. Run Callgrind on Q9 pipeline server
5. Generate code coverage reports with HTML output

### Individual Analysis Steps
```bash
# Q10: Memory analysis
make memcheck

# Q10: Thread safety analysis  
make helgrind

# Q10: Performance profiling
make callgrind

# Q11: Code coverage
make coverage coverage-html
```

## 📊 Output Structure

```
Q10toQ11/reports/
├── valgrind-memcheck/          # Q10: Memory leak reports
│   ├── q1_analysis.txt         # Q1-Q4 memory analysis
│   ├── q6_server.txt           # Q6 server memory analysis
│   ├── q7_server.txt           # Q7 server memory analysis  
│   ├── q8_server.txt           # Q8 server memory analysis
│   ├── q9_server.txt           # Q9 server memory analysis
│   └── *_session.txt           # Client interaction logs
├── valgrind-helgrind/          # Q10: Thread safety reports
│   ├── q8_server.txt           # Q8 Leader-Follower analysis
│   ├── q9_server.txt           # Q9 Pipeline analysis
│   └── *_session.txt           # Client interaction logs  
├── valgrind-callgrind/         # Q10: Performance profiling
│   ├── callgrind.q9.*.out      # Q9 performance data
│   └── q9_session.txt          # Client interaction log
└── coverage/                   # Q11: Coverage analysis
    ├── coverage.info           # Raw coverage data
    ├── coverage_filtered.info  # Filtered coverage data
    └── html/                   # HTML coverage report
        ├── index.html          # Main coverage report
        ├── Q1toQ4/             # Q1-Q4 coverage details
        ├── Q6/                 # Q6 coverage details
        ├── Q7/                 # Q7 coverage details
        ├── Q8/                 # Q8 coverage details
        └── Q9/                 # Q9 coverage details
```

## 🔍 Analysis Details

### Memory Analysis (Memcheck)
Tests all components for:
- Memory leaks
- Invalid memory reads/writes
- Use of uninitialized values
- Double free errors
- Buffer overflows

**Tested Components:**
- Q1: Graph analyzer with various input parameters
- Q6: Euler circuit server with client interactions
- Q7: Multi-algorithm server with all 4 algorithms
- Q8: Leader-Follower server with thread management
- Q9: Pipeline server with Active Objects