# OS Project - Graph Algorithms, Strategy/Factory, Client-Server, Threading
# Authors: Nitzan Wainshtein (209086263), Aviv Oz (207543927)

.PHONY: all build test analysis clean help

# Main targets
all: build analysis
	@echo "OS Project Build Complete"
	@echo "========================="
	@echo "Components: Q1-Q4 (Graph), Q6 (Server), Q7 (Algorithms), Q8 (LF), Q9 (Pipeline)"
	@echo "Analysis: Q10 (Valgrind), Q11 (Coverage)"

build: build-q1-q4 build-q6 build-q7 build-q8 build-q9
	@echo "All components built successfully"

test: test-q1-q4 test-q6 test-q7 test-q8 test-q9
	@echo "All tests completed"

analysis: build
	@echo "Running comprehensive analysis (Q10-Q11)..."
	@$(MAKE) -C Q10toQ11 all

# Individual component builds
build-q1-q4:
	@echo "Building Q1-Q4: Graph Data Structure & Euler Circuit"
	@$(MAKE) -C Q1toQ4 all

build-q6:
	@echo "Building Q6: Euler Circuit Server"
	@$(MAKE) -C Q6 all

build-q7:
	@echo "Building Q7: Multi-Algorithm Server (Strategy/Factory)"
	@$(MAKE) -C Q7 all

build-q8:
	@echo "Building Q8: Leader-Follower Server"
	@$(MAKE) -C Q8 all

build-q9:
	@echo "Building Q9: Pipeline Server (Active Objects)"
	@$(MAKE) -C Q9 all

# Individual component tests
test-q1-q4:
	@if [ -f Q1toQ4/graph_analyzer ]; then \
		echo "Testing Q1-Q4..."; \
		(cd Q1toQ4 && ./graph_analyzer --test); \
	fi

test-q6:
	@echo "Q6: Manual testing required - run server and client separately"

test-q7:
	@echo "Q7: Manual testing required - run server and client separately"

test-q8:
	@echo "Q8: Manual testing required - run server and client separately"

test-q9:
	@echo "Q9: Manual testing required - run server and client separately"

# Analysis targets
memcheck:
	@$(MAKE) -C Q10toQ11 memcheck

helgrind:
	@$(MAKE) -C Q10toQ11 helgrind

callgrind:
	@$(MAKE) -C Q10toQ11 callgrind

coverage:
	@$(MAKE) -C Q10toQ11 coverage

verify:
	@$(MAKE) -C Q10toQ11 verify

results:
	@$(MAKE) -C Q10toQ11 results

# Cleanup
clean:
	@echo "Cleaning all components..."
	@$(MAKE) -C Q1toQ4 clean 2>/dev/null || true
	@$(MAKE) -C Q6 clean 2>/dev/null || true
	@$(MAKE) -C Q7 clean 2>/dev/null || true
	@$(MAKE) -C Q8 clean 2>/dev/null || true
	@$(MAKE) -C Q9 clean 2>/dev/null || true
	@$(MAKE) -C Q10toQ11 clean 2>/dev/null || true
	@echo "Cleaning analysis artifacts..."
	@find . -name "*.gcov" -delete 2>/dev/null || true
	@find . -name "*.gcda" -delete 2>/dev/null || true
	@find . -name "*.gcno" -delete 2>/dev/null || true
	@find . -name "callgrind.out.*" -delete 2>/dev/null || true
	@find . -name "gmon.out" -delete 2>/dev/null || true
	@find . -name "*.txt.core.*" -delete 2>/dev/null || true
	@echo "Project cleaned completely"

# Help
help:
	@echo "OS Project - Graph Algorithms & Analysis"
	@echo "Authors: Nitzan Wainshtein (209086263), Aviv Oz (207543927)"
	@echo ""
	@echo "Main Targets:"
	@echo "  all        - Build all components and run analysis"
	@echo "  build      - Build all components"
	@echo "  test       - Run available tests"
	@echo "  analysis   - Run Q10-Q11 analysis (Valgrind + Coverage)"
	@echo ""
	@echo "Individual Components:"
	@echo "  build-q1-q4 - Graph data structure & Euler circuit"
	@echo "  build-q6    - Euler circuit server"
	@echo "  build-q7    - Multi-algorithm server (Strategy/Factory)"
	@echo "  build-q8    - Leader-Follower server"
	@echo "  build-q9    - Pipeline server (Active Objects)"
	@echo ""
	@echo "Analysis (Q10-Q11):"
	@echo "  memcheck   - Memory analysis"
	@echo "  helgrind   - Threading analysis"
	@echo "  callgrind  - Performance profiling"
	@echo "  coverage   - Code coverage"
	@echo "  verify     - Check requirements"
	@echo "  results    - View analysis summary"
	@echo ""
	@echo "Utility:"
	@echo "  clean      - Clean all build artifacts"
	@echo "  help       - Show this help"