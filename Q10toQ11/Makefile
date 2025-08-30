CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -g -O2 -pthread

# Project directories
Q1_DIR := ../Q1toQ4
Q6_DIR := ../Q6  
Q7_DIR := ../Q7
Q8_DIR := ../Q8
Q9_DIR := ../Q9

# Reports directory
REPORTS_DIR := reports
VALGRIND_DIR := $(REPORTS_DIR)/valgrind
COVERAGE_DIR := $(REPORTS_DIR)/coverage

# Coverage flags
COVERAGE_FLAGS := -g -O0 --coverage -fprofile-arcs -ftest-coverage

# Auto-detect binaries
Q1_BIN := $(shell find $(Q1_DIR) -name "graph_analyzer" -o -name "main" 2>/dev/null | head -1)
Q6_SERVER := $(shell find $(Q6_DIR) -name "euler_server" -o -name "server_main" 2>/dev/null | head -1)
Q6_CLIENT := $(shell find $(Q6_DIR) -name "euler_client" -o -name "client" 2>/dev/null | head -1)
Q7_SERVER := $(shell find $(Q7_DIR) -name "algorithms_server" -o -name "server_main" 2>/dev/null | head -1)
Q7_CLIENT := $(shell find $(Q7_DIR) -name "algorithms_client" -o -name "client" 2>/dev/null | head -1)
Q8_SERVER := $(shell find $(Q8_DIR) -name "lf_server" -o -name "server_main" 2>/dev/null | head -1)
Q8_CLIENT := $(shell find $(Q8_DIR) -name "lf_client" -o -name "client" 2>/dev/null | head -1)
Q9_SERVER := $(shell find $(Q9_DIR) -name "pipeline_server" -o -name "server_main" 2>/dev/null | head -1)
Q9_CLIENT := $(shell find $(Q9_DIR) -name "pipeline_client" -o -name "client" 2>/dev/null | head -1)

# Valgrind settings
VALGRIND := valgrind
MEMCHECK_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
HELGRIND_FLAGS := --tool=helgrind --verbose
CALLGRIND_FLAGS := --tool=callgrind --verbose

# Test ports
PORT_Q6 := 8080
PORT_Q7 := 8081  
PORT_Q8 := 8082
PORT_Q9 := 8083

.PHONY: all build-all q10 q11 memcheck helgrind callgrind coverage clean kill-servers results info

# ======================= MAIN TARGETS =======================

all: build-all q10 q11
	@echo ""
	@echo "Q10-Q11 Analysis Complete!"
	@echo "Q10: Valgrind analysis (memcheck, helgrind, callgrind)"
	@echo "Q11: Code coverage reports"
	@echo ""
	@echo "All reports available in: $(REPORTS_DIR)/"

q10: memcheck helgrind callgrind
	@echo "Q10 Valgrind Analysis Complete"

q11: coverage
	@echo "Q11 Code Coverage Complete"

# ======================= BUILD SYSTEM =======================

build-all:
	@echo "Building all projects for analysis..."
	@mkdir -p $(VALGRIND_DIR) $(COVERAGE_DIR)
	@if [ -d "$(Q1_DIR)" ]; then $(MAKE) -C $(Q1_DIR) clean all >/dev/null 2>&1 || echo "Warning: Q1-Q4 build issues"; fi
	@if [ -d "$(Q6_DIR)" ]; then $(MAKE) -C $(Q6_DIR) clean all >/dev/null 2>&1 || echo "Warning: Q6 build issues"; fi
	@if [ -d "$(Q7_DIR)" ]; then $(MAKE) -C $(Q7_DIR) clean all >/dev/null 2>&1 || echo "Warning: Q7 build issues"; fi
	@if [ -d "$(Q8_DIR)" ]; then $(MAKE) -C $(Q8_DIR) clean all >/dev/null 2>&1 || echo "Warning: Q8 build issues"; fi
	@if [ -d "$(Q9_DIR)" ]; then $(MAKE) -C $(Q9_DIR) clean all >/dev/null 2>&1 || echo "Warning: Q9 build issues"; fi
	@echo "Build complete"

# =================== Q10: VALGRIND ANALYSIS ===================

memcheck:
	@echo "Q10: Running Memcheck Analysis..."
	@mkdir -p $(VALGRIND_DIR)
	
	# Q1-Q4 Analysis
	@if [ -x "$(Q1_BIN)" ]; then \
		echo "  Testing Q1-Q4 Graph Analyzer"; \
		$(VALGRIND) $(MEMCHECK_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q1_memcheck.txt \
		$(Q1_BIN) -v 5 -e 6 -s 42 >/dev/null 2>&1; \
		$(VALGRIND) $(MEMCHECK_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q1_tests_memcheck.txt \
		$(Q1_BIN) --test >/dev/null 2>&1; \
		echo "    Q1 memcheck reports generated"; \
	else \
		echo "    Q1 binary not found"; \
	fi
	
	# Q6 Server Analysis
	@if [ -x "$(Q6_SERVER)" ] && [ -x "$(Q6_CLIENT)" ]; then \
		echo "  Testing Q6 Euler Server"; \
		$(VALGRIND) $(MEMCHECK_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q6_server_memcheck.txt \
		$(Q6_SERVER) $(PORT_Q6) & \
		SERVER_PID=$$!; \
		sleep 2; \
		echo -e "3 3 0-1 1-2 2-0\n4 4 0-1 1-2 2-3 3-0\nquit" | \
		$(Q6_CLIENT) localhost $(PORT_Q6) > $(VALGRIND_DIR)/q6_client_session.txt 2>&1; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q6 memcheck reports generated"; \
	else \
		echo "    Q6 binaries not found"; \
	fi
	
	# Q7 Server Analysis
	@if [ -x "$(Q7_SERVER)" ] && [ -x "$(Q7_CLIENT)" ]; then \
		echo "  Testing Q7 Multi-Algorithm Server"; \
		$(VALGRIND) $(MEMCHECK_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q7_server_memcheck.txt \
		$(Q7_SERVER) $(PORT_Q7) & \
		SERVER_PID=$$!; \
		sleep 2; \
		echo -e "euler 3 3 0-1 1-2 2-0\nclique 4 6 0-1 0-2 0-3 1-2 1-3 2-3\nmst 4 4 0-1 1-2 2-3 3-0\nhamilton 3 3 0-1 1-2 2-0\nquit" | \
		$(Q7_CLIENT) localhost $(PORT_Q7) > $(VALGRIND_DIR)/q7_client_session.txt 2>&1; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q7 memcheck reports generated"; \
	else \
		echo "    Q7 binaries not found"; \
	fi
	
	# Q8 Server Analysis  
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
		echo "  Testing Q8 Leader-Follower Server"; \
		$(VALGRIND) $(MEMCHECK_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q8_server_memcheck.txt \
		$(Q8_SERVER) $(PORT_Q8) & \
		SERVER_PID=$$!; \
		sleep 2; \
		echo -e "3 3 0-1 1-2 2-0\n4 4 0-1 1-2 2-3 3-0\nstats\nquit" | \
		$(Q8_CLIENT) localhost $(PORT_Q8) > $(VALGRIND_DIR)/q8_client_session.txt 2>&1; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q8 memcheck reports generated"; \
	else \
		echo "    Q8 binaries not found"; \
	fi
	
	# Q9 Server Analysis
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
		echo "  Testing Q9 Pipeline Server"; \
		$(VALGRIND) $(MEMCHECK_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q9_server_memcheck.txt \
		$(Q9_SERVER) $(PORT_Q9) & \
		SERVER_PID=$$!; \
		sleep 2; \
		echo -e "3 3 0-1 1-2 2-0\n4 6 0-1 0-2 0-3 1-2 1-3 2-3\nstats\nquit" | \
		$(Q9_CLIENT) localhost $(PORT_Q9) > $(VALGRIND_DIR)/q9_client_session.txt 2>&1; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q9 memcheck reports generated"; \
	else \
		echo "    Q9 binaries not found"; \
	fi
	
	@echo "Memcheck analysis complete: $(VALGRIND_DIR)/*_memcheck.txt"

helgrind:
	@echo "Q10: Running Helgrind Analysis..."
	@mkdir -p $(VALGRIND_DIR)
	
	# Q8 Threading Analysis (Leader-Follower)
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
		echo "  Testing Q8 Leader-Follower Threading"; \
		timeout 15 $(VALGRIND) $(HELGRIND_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q8_helgrind.txt \
		$(Q8_SERVER) $(PORT_Q8) & \
		SERVER_PID=$$!; \
		sleep 3; \
		echo -e "3 3 0-1 1-2 2-0\n4 4 0-1 1-2 2-3 3-0\nquit" | \
		$(Q8_CLIENT) localhost $(PORT_Q8) > $(VALGRIND_DIR)/q8_helgrind_session.txt 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q8 threading analysis complete"; \
	else \
		echo "    Q8 binaries not found"; \
	fi
	
	# Q9 Threading Analysis (Pipeline + Active Objects)
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
		echo "  Testing Q9 Pipeline Threading"; \
		timeout 15 $(VALGRIND) $(HELGRIND_FLAGS) \
		--log-file=$(VALGRIND_DIR)/q9_helgrind.txt \
		$(Q9_SERVER) $(PORT_Q9) & \
		SERVER_PID=$$!; \
		sleep 3; \
		echo -e "3 3 0-1 1-2 2-0\n4 6 0-1 0-2 0-3 1-2 1-3 2-3\nquit" | \
		$(Q9_CLIENT) localhost $(PORT_Q9) > $(VALGRIND_DIR)/q9_helgrind_session.txt 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q9 threading analysis complete"; \
	else \
		echo "    Q9 binaries not found"; \
	fi
	
	@echo "Helgrind analysis complete: $(VALGRIND_DIR)/*_helgrind.txt"

callgrind:
	@echo "Q10: Running Callgrind Analysis..."
	@mkdir -p $(VALGRIND_DIR)
	
	# Q9 Performance Analysis (most complex server)
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
		echo "  Profiling Q9 Pipeline Performance"; \
		timeout 20 $(VALGRIND) $(CALLGRIND_FLAGS) \
		--callgrind-out-file=$(VALGRIND_DIR)/callgrind.q9.out \
		$(Q9_SERVER) $(PORT_Q9) & \
		SERVER_PID=$$!; \
		sleep 4; \
		echo -e "3 3 0-1 1-2 2-0\n4 4 0-1 1-2 2-3 3-0\n4 6 0-1 0-2 0-3 1-2 1-3 2-3\n5 6 0-1 0-2 1-3 2-3 2-4 3-4\nquit" | \
		$(Q9_CLIENT) localhost $(PORT_Q9) > $(VALGRIND_DIR)/q9_callgrind_session.txt 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q9 performance profiling complete"; \
	else \
		echo "    Q9 binaries not found"; \
	fi
	
	# Q7 Performance Analysis (Strategy/Factory patterns)
	@if [ -x "$(Q7_SERVER)" ] && [ -x "$(Q7_CLIENT)" ]; then \
		echo "  Profiling Q7 Algorithm Performance"; \
		timeout 15 $(VALGRIND) $(CALLGRIND_FLAGS) \
		--callgrind-out-file=$(VALGRIND_DIR)/callgrind.q7.out \
		$(Q7_SERVER) $(PORT_Q7) & \
		SERVER_PID=$$!; \
		sleep 3; \
		echo -e "euler 4 4 0-1 1-2 2-3 3-0\nclique 4 6 0-1 0-2 0-3 1-2 1-3 2-3\nmst 5 8 0-1 0-2 1-2 1-3 2-3 2-4 3-4 0-4\nhamilton 4 4 0-1 1-2 2-3 3-0\nquit" | \
		$(Q7_CLIENT) localhost $(PORT_Q7) > $(VALGRIND_DIR)/q7_callgrind_session.txt 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "    Q7 performance profiling complete"; \
	else \
		echo "    Q7 binaries not found"; \
	fi
	
	@echo "Callgrind analysis complete: $(VALGRIND_DIR)/callgrind.*.out"

# ==================== Q11: CODE COVERAGE ====================

coverage:
	@echo "Q11: Running Code Coverage Analysis..."
	@mkdir -p $(COVERAGE_DIR)
	
	# Clean old coverage data
	@echo "  Cleaning old coverage data"
	@find .. -name "*.gcda" -delete 2>/dev/null || true
	@find .. -name "*.gcno" -delete 2>/dev/null || true
	@find .. -name "*.gcov" -delete 2>/dev/null || true
	
	# Build all projects with coverage
	@echo "  Building projects with coverage flags"
	@if [ -d "$(Q1_DIR)" ]; then $(MAKE) -C $(Q1_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q1_DIR)" ]; then $(MAKE) -C $(Q1_DIR) CXXFLAGS="$(CXXFLAGS) $(COVERAGE_FLAGS)" >/dev/null 2>&1 || echo "    Warning: Q1 coverage build failed"; fi
	
	@if [ -d "$(Q6_DIR)" ]; then $(MAKE) -C $(Q6_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q6_DIR)" ]; then $(MAKE) -C $(Q6_DIR) CXXFLAGS="$(CXXFLAGS) $(COVERAGE_FLAGS)" >/dev/null 2>&1 || echo "    Warning: Q6 coverage build failed"; fi
	
	@if [ -d "$(Q7_DIR)" ]; then $(MAKE) -C $(Q7_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q7_DIR)" ]; then $(MAKE) -C $(Q7_DIR) CXXFLAGS="$(CXXFLAGS) $(COVERAGE_FLAGS)" >/dev/null 2>&1 || echo "    Warning: Q7 coverage build failed"; fi
	
	@if [ -d "$(Q8_DIR)" ]; then $(MAKE) -C $(Q8_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q8_DIR)" ]; then $(MAKE) -C $(Q8_DIR) CXXFLAGS="$(CXXFLAGS) $(COVERAGE_FLAGS)" >/dev/null 2>&1 || echo "    Warning: Q8 coverage build failed"; fi
	
	@if [ -d "$(Q9_DIR)" ]; then $(MAKE) -C $(Q9_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q9_DIR)" ]; then $(MAKE) -C $(Q9_DIR) CXXFLAGS="$(CXXFLAGS) $(COVERAGE_FLAGS)" >/dev/null 2>&1 || echo "    Warning: Q9 coverage build failed"; fi
	
	# Run comprehensive tests for coverage
	@echo "  Running comprehensive test scenarios"
	
	# Q1 Tests
	@if [ -x "$(Q1_BIN)" ]; then \
		echo "    Testing Q1-Q4..."; \
		(cd $(Q1_DIR) && ./`basename $(Q1_BIN)` --test >/dev/null 2>&1 || true); \
		(cd $(Q1_DIR) && ./`basename $(Q1_BIN)` -v 3 -e 3 -s 42 >/dev/null 2>&1 || true); \
		(cd $(Q1_DIR) && ./`basename $(Q1_BIN)` -v 5 -e 7 -s 123 >/dev/null 2>&1 || true); \
		(cd $(Q1_DIR) && ./`basename $(Q1_BIN)` -v 4 -e 6 -s 456 -d >/dev/null 2>&1 || true); \
		echo "      Q1 coverage data collected"; \
	fi
	
	# Q6 Tests
	@if [ -x "$(Q6_SERVER)" ] && [ -x "$(Q6_CLIENT)" ]; then \
		echo "    Testing Q6..."; \
		$(Q6_SERVER) $(PORT_Q6) & SERVER_PID=$$!; \
		sleep 1; \
		echo -e "3 3 0-1 1-2 2-0\n4 4 0-1 1-2 2-3 3-0\n3 2 0-1 1-2\nhelp\nquit" | \
		$(Q6_CLIENT) localhost $(PORT_Q6) >/dev/null 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "      Q6 coverage data collected"; \
	fi
	
	# Q7 Tests
	@if [ -x "$(Q7_SERVER)" ] && [ -x "$(Q7_CLIENT)" ]; then \
		echo "    Testing Q7..."; \
		$(Q7_SERVER) $(PORT_Q7) & SERVER_PID=$$!; \
		sleep 1; \
		echo -e "euler 3 3 0-1 1-2 2-0\nclique 4 6 0-1 0-2 0-3 1-2 1-3 2-3\nmst 4 4 0-1 1-2 2-3 3-0\nhamilton 3 3 0-1 1-2 2-0\nhelp\nquit" | \
		$(Q7_CLIENT) localhost $(PORT_Q7) >/dev/null 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "      Q7 coverage data collected"; \
	fi
	
	# Q8 Tests
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
		echo "    Testing Q8..."; \
		$(Q8_SERVER) $(PORT_Q8) & SERVER_PID=$$!; \
		sleep 1; \
		echo -e "3 3 0-1 1-2 2-0\n4 4 0-1 1-2 2-3 3-0\nstats\nhelp\nquit" | \
		$(Q8_CLIENT) localhost $(PORT_Q8) >/dev/null 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "      Q8 coverage data collected"; \
	fi
	
	# Q9 Tests  
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
		echo "    Testing Q9..."; \
		$(Q9_SERVER) $(PORT_Q9) & SERVER_PID=$$!; \
		sleep 1; \
		echo -e "3 3 0-1 1-2 2-0\n4 6 0-1 0-2 0-3 1-2 1-3 2-3\nstats\nhelp\nquit" | \
		$(Q9_CLIENT) localhost $(PORT_Q9) >/dev/null 2>&1 || true; \
		kill $$SERVER_PID 2>/dev/null || true; \
		wait $$SERVER_PID 2>/dev/null || true; \
		echo "      Q9 coverage data collected"; \
	fi
	
	# Generate .gcov files from .gcda data
	@echo "  Generating .gcov files"
	@for project in Q1toQ4 Q6 Q7 Q8 Q9; do \
		if [ -d "../$$project" ]; then \
			echo "    Processing $$project coverage..."; \
			(cd ../$$project && gcov *.gcda >/dev/null 2>&1 || true); \
		fi; \
	done
	
	# Copy all .gcov files to coverage directory
	@echo "  Collecting .gcov files"
	@find .. -name "*.gcov" -exec cp {} $(COVERAGE_DIR)/ \; 2>/dev/null || true
	
	# Generate coverage summary
	@echo "Coverage Analysis Results - Q10-Q11" > $(COVERAGE_DIR)/coverage_summary.txt
	@echo "====================================" >> $(COVERAGE_DIR)/coverage_summary.txt
	@echo "Generated: `date`" >> $(COVERAGE_DIR)/coverage_summary.txt
	@echo "" >> $(COVERAGE_DIR)/coverage_summary.txt
	@echo "Coverage Files Collected:" >> $(COVERAGE_DIR)/coverage_summary.txt
	@ls $(COVERAGE_DIR)/*.gcov 2>/dev/null | wc -l | sed 's/^/Total .gcov files: /' >> $(COVERAGE_DIR)/coverage_summary.txt
	@echo "" >> $(COVERAGE_DIR)/coverage_summary.txt
	@echo "Sample Coverage Results:" >> $(COVERAGE_DIR)/coverage_summary.txt
	@if [ -f $(COVERAGE_DIR)/Graph.cpp.gcov ]; then head -5 $(COVERAGE_DIR)/Graph.cpp.gcov >> $(COVERAGE_DIR)/coverage_summary.txt; fi
	
	@gcov_count=`ls $(COVERAGE_DIR)/*.gcov 2>/dev/null | wc -l`
	@echo "Code coverage complete: Generated $$gcov_count coverage files"
	@echo "Coverage reports: $(COVERAGE_DIR)/"
	@echo "Summary: $(COVERAGE_DIR)/coverage_summary.txt"

# ======================== UTILITIES ========================

kill-servers:
	@echo "Killing any running servers..."
	@pkill -f "euler_server|algorithms_server|lf_server|pipeline_server|graph_analyzer" 2>/dev/null || true
	@sleep 1

clean: kill-servers
	@echo "Cleaning all analysis artifacts..."
	@rm -rf $(REPORTS_DIR)
	@find .. -name "*.gcno" -delete 2>/dev/null || true
	@find .. -name "*.gcda" -delete 2>/dev/null || true
	@find .. -name "*.gcov" -delete 2>/dev/null || true
	@find .. -name "callgrind.out.*" -delete 2>/dev/null || true
	@echo "  Cleaning project builds..."
	@if [ -d "$(Q1_DIR)" ]; then $(MAKE) -C $(Q1_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q6_DIR)" ]; then $(MAKE) -C $(Q6_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q7_DIR)" ]; then $(MAKE) -C $(Q7_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q8_DIR)" ]; then $(MAKE) -C $(Q8_DIR) clean >/dev/null 2>&1 || true; fi
	@if [ -d "$(Q9_DIR)" ]; then $(MAKE) -C $(Q9_DIR) clean >/dev/null 2>&1 || true; fi
	@echo "Complete cleanup finished"

results:
	@echo "Q10-Q11 Analysis Results Summary"
	@echo "================================"
	@if [ -d "$(REPORTS_DIR)" ]; then \
		echo ""; \
		echo "Reports Directory: $(REPORTS_DIR)/"; \
		echo ""; \
		echo "Q10: Valgrind Analysis"; \
		echo "----------------------"; \
		if [ -d "$(VALGRIND_DIR)" ]; then \
			valgrind_count=`ls $(VALGRIND_DIR)/*.txt 2>/dev/null | wc -l`; \
			callgrind_count=`ls $(VALGRIND_DIR)/*.out 2>/dev/null | wc -l`; \
			echo "  Memcheck/Helgrind reports: $$valgrind_count"; \
			echo "  Callgrind profiles: $$callgrind_count"; \
		else \
			echo "  No Valgrind reports found"; \
		fi; \
		echo ""; \
		echo "Q11: Code Coverage"; \
		echo "------------------"; \
		if [ -d "$(COVERAGE_DIR)" ]; then \
			gcov_count=`ls $(COVERAGE_DIR)/*.gcov 2>/dev/null | wc -l`; \
			echo "  Coverage files: $$gcov_count"; \
			echo "  Summary: $(COVERAGE_DIR)/coverage_summary.txt"; \
		else \
			echo "  No coverage reports found"; \
		fi; \
	else \
		echo ""; \
		echo "No reports found. Run 'make all' first."; \
	fi

verify:
	@echo "Q10-Q11 Requirements Verification"
	@echo "================================="
	@echo "Q10 Memcheck: `[ -f $(VALGRIND_DIR)/q1_memcheck.txt ] && echo 'PASS' || echo 'FAIL'`"
	@echo "Q10 Helgrind: `[ -f $(VALGRIND_DIR)/q8_helgrind.txt ] && echo 'PASS' || echo 'FAIL'`"
	@echo "Q10 Callgrind: `[ -f $(VALGRIND_DIR)/callgrind.q9.out ] && echo 'PASS' || echo 'FAIL'`"
	@gcov_count=`ls $(COVERAGE_DIR)/*.gcov 2>/dev/null | wc -l`; \
	echo "Q11 Coverage: `[ $$gcov_count -gt 10 ] && echo 'PASS ($$gcov_count files)' || echo 'FAIL ($$gcov_count files)'`"

info:
	@echo "Q10-Q11 Analysis System"
	@echo "======================="
	@echo ""
	@echo "Targets:"
	@echo "  all       - Complete Q10+Q11 analysis"
	@echo "  q10       - Valgrind analysis only"
	@echo "  q11       - Code coverage only"  
	@echo "  memcheck  - Memory leak detection"
	@echo "  helgrind  - Threading analysis"
	@echo "  callgrind - Performance profiling"
	@echo "  coverage  - Code coverage reports"
	@echo "  results   - Show analysis summary"
	@echo "  verify    - Check requirements status"
	@echo "  clean     - Remove all reports"