SHELL := /bin/bash
CXX   := g++

# ---- Project directories ----
Q1_DIR := ../Q1toQ4
Q6_DIR := ../Q6
Q7_DIR := ../Q7
Q8_DIR := ../Q8
Q9_DIR := ../Q9

# ---- Reports directory ----
REPORT_DIR    := reports
MEMCHECK_DIR  := $(REPORT_DIR)/valgrind-memcheck
HELGRIND_DIR  := $(REPORT_DIR)/valgrind-helgrind
CALLGRIND_DIR := $(REPORT_DIR)/valgrind-callgrind
COV_DIR       := $(REPORT_DIR)/coverage

# ---- Auto-detect binaries ----
Q1_BIN ?= $(shell for c in graph_analyzer main; do for d in ../Q1toQ4 ../Q1-Q4; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q6_SERVER ?= $(shell for c in euler_server server_main; do for d in ../Q6; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q6_CLIENT ?= $(shell for c in euler_client client; do for d in ../Q6; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q7_SERVER ?= $(shell for c in algorithms_server server_main; do for d in ../Q7; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q7_CLIENT ?= $(shell for c in algorithms_client client; do for d in ../Q7; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q8_SERVER ?= $(shell for c in lf_server server_main; do for d in ../Q8; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q8_CLIENT ?= $(shell for c in lf_client client; do for d in ../Q8; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q9_SERVER ?= $(shell for c in pipeline_server server_main; do for d in ../Q9; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)
Q9_CLIENT ?= $(shell for c in pipeline_client client; do for d in ../Q9; do p="$$d/$$c"; [ -x "$$p" ] && echo "$$p" && break 2; done; done)

# ---- Ports for testing ----
PORT_Q6 ?= 8080
PORT_Q7 ?= 8081
PORT_Q8 ?= 8082
PORT_Q9 ?= 8083

# ---- Coverage flags (basic gcov only) ----
COVERAGE_FLAGS := -g -O0 --coverage -fprofile-arcs -ftest-coverage

# ---- Valgrind settings ----
VALGRIND := valgrind
MEMCHECK_FLAGS  := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=0
HELGRIND_FLAGS  := --tool=helgrind
CALLGRIND_FLAGS := --tool=callgrind

.PHONY: all build-all prep memcheck helgrind callgrind coverage clean clean-coverage kill-servers

# ======================= MAIN TARGET =======================
all: build-all memcheck helgrind callgrind coverage
	@echo ""
	@echo "🎉 Q10-Q11 Analysis Complete!"
	@echo "Q10: ✅ Valgrind memcheck, helgrind, callgrind"
	@echo "Q11: ✅ Code coverage reports (gcov)"
	@echo ""
	@echo "📁 Reports in: $(REPORT_DIR)/"

# ======================= BUILD SYSTEM =======================
build-all: prep
	@echo "🔨 Building all projects..."
	@for proj in Q1toQ4 Q6 Q7 Q8 Q9; do \
	  if [ -d "../$$proj" ]; then \
	    echo "  Building $$proj..."; \
	    $(MAKE) -C "../$$proj" clean >/dev/null 2>&1 || true; \
	    $(MAKE) -C "../$$proj" >/dev/null 2>&1 || echo "    Warning: $$proj build failed"; \
	  fi; \
	done

prep:
	@mkdir -p $(MEMCHECK_DIR) $(HELGRIND_DIR) $(CALLGRIND_DIR) $(COV_DIR)

# ===================== Q10: MEMCHECK ========================
memcheck: 
	@echo "🔍 Q10: Running Memcheck analysis..."
	
	@if [ -x "$(Q1_BIN)" ]; then \
	  echo "  Testing Q1..."; \
	  $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=reports/valgrind-memcheck/q1_memcheck.txt \
	    $(Q1_BIN) -v 6 -e 7 -s 42 >/dev/null 2>&1 || true; \
	fi
	
	@if [ -x "$(Q6_SERVER)" ] && [ -x "$(Q6_CLIENT)" ]; then \
	  echo "  Testing Q6..."; \
	  ( $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=reports/valgrind-memcheck/q6_memcheck.txt \
	      $(Q6_SERVER) $(PORT_Q6) & echo $$! > .q6.pid ); \
	  sleep 1.5; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q6_CLIENT) localhost $(PORT_Q6) >/dev/null 2>&1 || true; \
	  kill `cat .q6.pid` 2>/dev/null || true; rm -f .q6.pid; \
	fi
	
	@if [ -x "$(Q7_SERVER)" ] && [ -x "$(Q7_CLIENT)" ]; then \
	  echo "  Testing Q7..."; \
	  ( $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=reports/valgrind-memcheck/q7_memcheck.txt \
	      $(Q7_SERVER) $(PORT_Q7) & echo $$! > .q7.pid ); \
	  sleep 1.5; \
	  echo -e "euler 3 3 0-1 1-2 2-0\nquit\n" | $(Q7_CLIENT) localhost $(PORT_Q7) >/dev/null 2>&1 || true; \
	  kill `cat .q7.pid` 2>/dev/null || true; rm -f .q7.pid; \
	fi
	
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
	  echo "  Testing Q8..."; \
	  ( $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=reports/valgrind-memcheck/q8_memcheck.txt \
	      $(Q8_SERVER) $(PORT_Q8) & echo $$! > .q8.pid ); \
	  sleep 1.5; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q8_CLIENT) localhost $(PORT_Q8) >/dev/null 2>&1 || true; \
	  kill `cat .q8.pid` 2>/dev/null || true; rm -f .q8.pid; \
	fi
	
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  echo "  Testing Q9..."; \
	  ( $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=reports/valgrind-memcheck/q9_memcheck.txt \
	      $(Q9_SERVER) $(PORT_Q9) & echo $$! > .q9.pid ); \
	  sleep 1.5; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q9_CLIENT) localhost $(PORT_Q9) >/dev/null 2>&1 || true; \
	  kill `cat .q9.pid` 2>/dev/null || true; rm -f .q9.pid; \
	fi
	
	@echo "✅ Memcheck reports: $(MEMCHECK_DIR)/"

# ===================== Q10: HELGRIND ========================
helgrind:
	@echo "🧵 Q10: Running Helgrind analysis..."
	
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
	  echo "  Testing Q8 threading..."; \
	  ( timeout 10 $(VALGRIND) $(HELGRIND_FLAGS) --log-file=reports/valgrind-helgrind/q8_helgrind.txt \
	      $(Q8_SERVER) $(PORT_Q8) & echo $$! > .q8h.pid ); \
	  sleep 2; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q8_CLIENT) localhost $(PORT_Q8) >/dev/null 2>&1 || true; \
	  kill `cat .q8h.pid` 2>/dev/null || true; rm -f .q8h.pid; \
	fi
	
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  echo "  Testing Q9 threading..."; \
	  ( timeout 10 $(VALGRIND) $(HELGRIND_FLAGS) --log-file=reports/valgrind-helgrind/q9_helgrind.txt \
	      $(Q9_SERVER) $(PORT_Q9) & echo $$! > .q9h.pid ); \
	  sleep 2; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q9_CLIENT) localhost $(PORT_Q9) >/dev/null 2>&1 || true; \
	  kill `cat .q9h.pid` 2>/dev/null || true; rm -f .q9h.pid; \
	fi
	
	@echo "✅ Helgrind reports: $(HELGRIND_DIR)/"

# ===================== Q10: CALLGRIND =======================
callgrind:
	@echo "⚡ Q10: Running Callgrind analysis..."
	
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  echo "  Profiling Q9..."; \
	  ( timeout 15 $(VALGRIND) $(CALLGRIND_FLAGS) --callgrind-out-file=reports/valgrind-callgrind/callgrind.q9.out \
	      $(Q9_SERVER) $(PORT_Q9) & echo $$! > .q9cg.pid ); \
	  sleep 3; \
	  echo -e "3 3 0-1 1-2 2-0\n4 6 0-1 0-2 0-3 1-2 1-3 2-3\nquit\n" | \
	    $(Q9_CLIENT) localhost $(PORT_Q9) >/dev/null 2>&1 || true; \
	  sleep 1; \
	  kill `cat .q9cg.pid` 2>/dev/null || true; rm -f .q9cg.pid; \
	fi
	
	@echo "✅ Callgrind reports: $(CALLGRIND_DIR)/"
	@echo "   Analyze with: callgrind_annotate $(CALLGRIND_DIR)/callgrind.q9.out"

# ====================== Q11: COVERAGE =======================
coverage:
	@echo "📊 Q11: Running Coverage analysis..."
	@echo "  Rebuilding with coverage flags..."
	
	# Clean old coverage data
	@find .. -name "*.gcda" -o -name "*.gcno" -delete 2>/dev/null || true
	
	# Build with coverage
	@for proj in Q1toQ4 Q6 Q7 Q8 Q9; do \
	  if [ -d "../$$proj" ]; then \
	    echo "    Coverage build: $$proj"; \
	    $(MAKE) -C "../$$proj" clean >/dev/null 2>&1 || true; \
	    $(MAKE) -C "../$$proj" CXXFLAGS+=" $(COVERAGE_FLAGS)" >/dev/null 2>&1 || true; \
	  fi; \
	done
	
	@echo "  Running test scenarios..."
	
	# Q1 tests
	@if [ -x "$(Q1_BIN)" ]; then \
	  echo "    Testing Q1..."; \
	  (cd $(Q1_DIR) && ./`basename $(Q1_BIN)` --test >/dev/null 2>&1 || true); \
	  (cd $(Q1_DIR) && ./`basename $(Q1_BIN)` -v 5 -e 6 -s 42 >/dev/null 2>&1 || true); \
	fi
	
	# Q6 tests
	@if [ -x "$(Q6_SERVER)" ] && [ -x "$(Q6_CLIENT)" ]; then \
	  echo "    Testing Q6..."; \
	  ( $(Q6_SERVER) $(PORT_Q6) & echo $$! > .q6c.pid ); \
	  sleep 0.5; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q6_CLIENT) localhost $(PORT_Q6) >/dev/null 2>&1 || true; \
	  kill `cat .q6c.pid` 2>/dev/null || true; rm -f .q6c.pid; \
	fi
	
	# Q7 tests  
	@if [ -x "$(Q7_SERVER)" ] && [ -x "$(Q7_CLIENT)" ]; then \
	  echo "    Testing Q7..."; \
	  ( $(Q7_SERVER) $(PORT_Q7) & echo $$! > .q7c.pid ); \
	  sleep 0.5; \
	  echo -e "euler 3 3 0-1 1-2 2-0\nclique 4 6 0-1 0-2 0-3 1-2 1-3 2-3\nquit\n" | $(Q7_CLIENT) localhost $(PORT_Q7) >/dev/null 2>&1 || true; \
	  kill `cat .q7c.pid` 2>/dev/null || true; rm -f .q7c.pid; \
	fi
	
	# Q8 tests
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
	  echo "    Testing Q8..."; \
	  ( $(Q8_SERVER) $(PORT_Q8) & echo $$! > .q8c.pid ); \
	  sleep 0.5; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q8_CLIENT) localhost $(PORT_Q8) >/dev/null 2>&1 || true; \
	  kill `cat .q8c.pid` 2>/dev/null || true; rm -f .q8c.pid; \
	fi
	
	# Q9 tests
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  echo "    Testing Q9..."; \
	  ( $(Q9_SERVER) $(PORT_Q9) & echo $$! > .q9c.pid ); \
	  sleep 0.5; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q9_CLIENT) localhost $(PORT_Q9) >/dev/null 2>&1 || true; \
	  kill `cat .q9c.pid` 2>/dev/null || true; rm -f .q9c.pid; \
	fi
	
	@echo "  Generating coverage reports..."
	@mkdir -p $(COV_DIR)
	@echo "Collecting gcov files from all projects..." > $(COV_DIR)/gcov_output.txt
	@cd .. && find . -name "*.gcda" -exec gcov {} \; >> $(COV_DIR)/gcov_output.txt 2>&1 || true
	@cd .. && find . -name "*.gcov" -exec cp {} $(COV_DIR)/ \; 2>/dev/null || true
	
	@echo "✅ Coverage reports: $(COV_DIR)/"
	@gcov_count=$$(ls $(COV_DIR)/*.gcov 2>/dev/null | wc -l); \
	echo "   Generated $$gcov_count .gcov files"
	@echo "   Summary report: $(COV_DIR)/gcov_output.txt"

# ======================== UTILITIES ========================
kill-servers:
	@-pkill -f "euler_server|algorithms_server|lf_server|pipeline_server" 2>/dev/null || true

clean: kill-servers
	@echo "🧹 Cleaning all reports and coverage artifacts..."
	@rm -rf $(REPORT_DIR)
	@echo "  Cleaning coverage files from all project directories..."
	@find .. -name "*.gcno" -delete 2>/dev/null || true
	@find .. -name "*.gcda" -delete 2>/dev/null || true  
	@find .. -name "*.gcov" -delete 2>/dev/null || true
	@find .. -name "callgrind.out.*" -delete 2>/dev/null || true
	@echo "  Cleaning build artifacts in each project..."
	@for proj in Q1toQ4 Q6 Q7 Q8 Q9; do \
	  if [ -d "../$$proj" ]; then \
	    $(MAKE) -C "../$$proj" clean >/dev/null 2>&1 || true; \
	  fi; \
	done
	@echo "✅ Complete cleanup done"

clean-coverage:
	@echo "🧹 Cleaning only coverage files..."
	@find .. -name "*.gcno" -delete 2>/dev/null || true
	@find .. -name "*.gcda" -delete 2>/dev/null || true  
	@find .. -name "*.gcov" -delete 2>/dev/null || true
	@rm -rf $(COV_DIR)
	@echo "✅ Coverage cleanup done"

info:
	@echo "📋 Q10-Q11 Simple Analysis (Exact Requirements)"
	@echo "Q10: Valgrind memcheck, helgrind, callgrind"
	@echo "Q11: Code coverage reports (gcov)"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Complete analysis"  
	@echo "  memcheck  - Memory analysis"
	@echo "  helgrind  - Thread analysis" 
	@echo "  callgrind - Performance profiling"
	@echo "  coverage  - Code coverage (gcov)"
	@echo "  clean     - Clean reports"