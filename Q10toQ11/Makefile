SHELL := /bin/bash
CXX   := g++

# ---- Project dirs ----
Q1_DIR := ../Q1toQ4
Q6_DIR := ../Q6
Q7_DIR := ../Q7
Q8_DIR := ../Q8
Q9_DIR := ../Q9

# ---- Auto-detect binaries (עודכנו שמות) ----
Q1_BIN ?= $(shell for c in graph_analyzer q1 q1_cli main; do p="$(Q1_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)

Q6_SERVER ?= $(shell for c in euler_server server_main server; do p="$(Q6_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)
Q6_CLIENT ?= $(shell for c in euler_client client;         do p="$(Q6_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)

Q7_SERVER ?= $(shell for c in algorithms_server server_main alg_server server; do p="$(Q7_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)
Q7_CLIENT ?= $(shell for c in algorithms_client client alg_client;            do p="$(Q7_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)

Q8_SERVER ?= $(shell for c in lf_server server_main server; do p="$(Q8_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)
Q8_CLIENT ?= $(shell for c in lf_client client;             do p="$(Q8_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)

Q9_SERVER ?= $(shell for c in pipeline_server server_main; do p="$(Q9_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)
Q9_CLIENT ?= $(shell for c in pipeline_client client;      do p="$(Q9_DIR)/$$c"; [ -x "$$p" ] && echo "$$p" && break; done)

# ---- Reports ----
REPORT_DIR    := reports
MEMCHECK_DIR  := $(REPORT_DIR)/valgrind-memcheck
HELGRIND_DIR  := $(REPORT_DIR)/valgrind-helgrind
CALLGRIND_DIR := $(REPORT_DIR)/valgrind-callgrind
COV_DIR       := $(REPORT_DIR)/coverage
COV_HTML      := $(COV_DIR)/html

# ---- Ports (אפשר לשנות עם VAR=PORT_Q9=8090) ----
PORT_Q6 ?= 8080
PORT_Q7 ?= 8081
PORT_Q8 ?= 8082
PORT_Q9 ?= 8083

# ---- Coverage flags ----
COVERAGE_CXXFLAGS := -O0 -g --coverage -fprofile-arcs -ftest-coverage

# ---- Valgrind ----
VALGRIND := valgrind
MEMCHECK_FLAGS  := --leak-check=full --show-leak-kinds=all --track-origins=yes --num-callers=50 --error-exitcode=1
HELGRIND_FLAGS  := --tool=helgrind --history-level=full --ignore-races=yes
CALLGRIND_FLAGS := --tool=callgrind --callgrind-out-file=$(CALLGRIND_DIR)/callgrind-%p.out

# ---- Q9 manual build (כדי לשמר include paths) ----
Q9_INCLUDES := -I$(Q9_DIR) -I$(Q1_DIR) -I$(Q7_DIR)
Q9_CXXFLAGS := -std=c++17 -pthread -Wall -Wextra $(COVERAGE_CXXFLAGS)
Q9_SRCS_SRV := $(Q9_DIR)/server_main.cpp $(Q9_DIR)/PipelineServer.cpp \
               $(Q1_DIR)/Graph.cpp \
               $(Q7_DIR)/AlgorithmFactory.cpp $(Q7_DIR)/EulerAlgorithm.cpp \
               $(Q7_DIR)/Max_clique_algorithm.cpp $(Q7_DIR)/Mst_algorithm.cpp $(Q7_DIR)/hamilton_algorithm.cpp
Q9_SRCS_CLI := $(Q9_DIR)/client.cpp

.PHONY: all report build-all prep memcheck helgrind callgrind \
        coverage coverage-rebuild coverage-html kill-servers clean \
        preflight pre_coverage_clean

all: report

report: build-all memcheck helgrind callgrind coverage coverage-html
	@echo -e "\n✅ Q10–Q11 completed."
	@echo "• Memcheck logs:   $(MEMCHECK_DIR)"
	@echo "• Helgrind logs:   $(HELGRIND_DIR)"
	@echo "• Callgrind files: $(CALLGRIND_DIR)"
	@echo "• Coverage HTML:   $(COV_HTML)/index.html"

# ======================= Build (release) ====================
build-all: prep
	@echo "==> Building all subprojects (release)"
	@$(MAKE) -C $(Q1_DIR) clean || true; $(MAKE) -C $(Q1_DIR)
	@$(MAKE) -C $(Q6_DIR) clean || true; $(MAKE) -C $(Q6_DIR)
	@$(MAKE) -C $(Q7_DIR) clean || true; $(MAKE) -C $(Q7_DIR)
	@$(MAKE) -C $(Q8_DIR) clean || true; $(MAKE) -C $(Q8_DIR)
	@$(MAKE) -C $(Q9_DIR) clean || true; $(MAKE) -C $(Q9_DIR)

prep:
	@mkdir -p $(MEMCHECK_DIR) $(HELGRIND_DIR) $(CALLGRIND_DIR) $(COV_DIR) $(COV_HTML)

# ===================== Q10: memcheck ========================
memcheck: memcheck-q1 memcheck-q6 memcheck-q8 memcheck-q9
	@echo "==> Memcheck done."

memcheck-q1:
	@echo "==> Memcheck Q1–Q4"
	@if [ -x "$(Q1_BIN)" ]; then \
	  $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=$(abspath $(MEMCHECK_DIR))/q1_cli.txt \
	    $(Q1_BIN) -v 6 -e 7 -s 42 >/dev/null 2>&1 || true; \
	else echo "(!) Skipping Q1 memcheck: binary not found"; fi

memcheck-q6:
	@echo "==> Memcheck Q6"
	@if [ -x "$(Q6_SERVER)" ] && [ -x "$(Q6_CLIENT)" ]; then \
	  cd $(Q6_DIR) && \
	  ( $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=$(abspath $(MEMCHECK_DIR))/q6_server.txt \
	      $(Q6_SERVER) $(PORT_Q6) & echo $$! > .q6.pid ); \
	  sleep 0.6; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q6_CLIENT) localhost $(PORT_Q6) > $(abspath $(MEMCHECK_DIR))/q6_session.txt || true; \
	  kill `cat $(Q6_DIR)/.q6.pid` 2>/dev/null || true; rm -f $(Q6_DIR)/.q6.pid; \
	else echo "(!) Skipping Q6 memcheck: missing binaries"; fi

memcheck-q8:
	@echo "==> Memcheck Q8"
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
	  cd $(Q8_DIR) && \
	  ( $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=$(abspath $(MEMCHECK_DIR))/q8_server.txt \
	      $(Q8_SERVER) $(PORT_Q8) & echo $$! > .q8.pid ); \
	  sleep 0.6; \
	  echo -e "help\n3 3 0-1 1-2 2-0\nstats\nquit\n" | $(Q8_CLIENT) localhost $(PORT_Q8) > $(abspath $(MEMCHECK_DIR))/q8_session.txt || true; \
	  kill `cat $(Q8_DIR)/.q8.pid` 2>/dev/null || true; rm -f $(Q8_DIR)/.q8.pid; \
	else echo "(!) Skipping Q8 memcheck: missing binaries"; fi

memcheck-q9:
	@echo "==> Memcheck Q9"
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  cd $(Q9_DIR) && \
	  ( $(VALGRIND) $(MEMCHECK_FLAGS) --log-file=$(abspath $(MEMCHECK_DIR))/q9_server.txt \
	      $(Q9_SERVER) $(PORT_Q9) & echo $$! > .q9.pid ); \
	  sleep 0.6; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q9_CLIENT) localhost $(PORT_Q9) > $(abspath $(MEMCHECK_DIR))/q9_session.txt || true; \
	  kill `cat $(Q9_DIR)/.q9.pid` 2>/dev/null || true; rm -f $(Q9_DIR)/.q9.pid; \
	else echo "(!) Skipping Q9 memcheck: missing binaries"; fi

# ===================== Q10: helgrind ========================
helgrind: helgrind-q8 helgrind-q9
	@echo "==> Helgrind done."

helgrind-q8:
	@echo "==> Helgrind Q8"
	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
	  cd $(Q8_DIR) && \
	  ( $(VALGRIND) $(HELGRIND_FLAGS) --log-file=$(abspath $(HELGRIND_DIR))/q8_server.txt \
	      $(Q8_SERVER) $(PORT_Q8) & echo $$! > .q8h.pid ); \
	  sleep 0.6; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q8_CLIENT) localhost $(PORT_Q8) > $(abspath $(HELGRIND_DIR))/q8_session.txt || true; \
	  kill `cat $(Q8_DIR)/.q8h.pid` 2>/dev/null || true; rm -f $(Q8_DIR)/.q8h.pid; \
	else echo "(!) Skipping Q8 helgrind: missing binaries"; fi

helgrind-q9:
	@echo "==> Helgrind Q9"
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  cd $(Q9_DIR) && \
	  ( $(VALGRIND) $(HELGRIND_FLAGS) --log-file=$(abspath $(HELGRIND_DIR))/q9_server.txt \
	      $(Q9_SERVER) $(PORT_Q9) & echo $$! > .q9h.pid ); \
	  sleep 0.6; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q9_CLIENT) localhost $(PORT_Q9) > $(abspath $(HELGRIND_DIR))/q9_session.txt || true; \
	  kill `cat $(Q9_DIR)/.q9h.pid` 2>/dev/null || true; rm -f $(Q9_DIR)/.q9h.pid; \
	else echo "(!) Skipping Q9 helgrind: missing binaries"; fi

# ===================== Q10: callgrind =======================
callgrind: callgrind-q9
	@echo "==> Callgrind done."

callgrind-q9:
	@echo "==> Callgrind Q9"
	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  cd $(Q9_DIR) && \
	  ( $(VALGRIND) $(CALLGRIND_FLAGS) $(Q9_SERVER) $(PORT_Q9) & echo $$! > .q9cg.pid ); \
	  sleep 0.6; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | $(Q9_CLIENT) localhost $(PORT_Q9) > $(abspath $(CALLGRIND_DIR))/q9_session.txt || true; \
	  kill `cat $(Q9_DIR)/.q9cg.pid` 2>/dev/null || true; rm -f $(Q9_DIR)/.q9cg.pid; \
	  echo "   -> Analyze with: kcachegrind $(CALLGRIND_DIR)/callgrind-*.out"; \
	else echo "(!) Skipping Q9 callgrind: missing binaries"; fi

# ====================== Q11: Coverage =======================
coverage: preflight coverage-rebuild
	@echo "==> Running coverage scenarios (Q1, Q6, Q7, Q8, Q9)"
	@rm -rf $(COV_DIR)/*; mkdir -p $(COV_HTML)

	@if [ -x "$(Q1_BIN)" ]; then \
	  (cd $(Q1_DIR) && ./`basename $(Q1_BIN)` -v 6 -e 7 -s 42 >/dev/null 2>&1 || true); \
	else echo "(!) Skipping Q1 coverage run: binary not found"; fi

	@if [ -x "$(Q6_SERVER)" ] && [ -x "$(Q6_CLIENT)" ]; then \
	  cd $(Q6_DIR) && ( ./`basename $(Q6_SERVER)` $(PORT_Q6) & echo $$! > .q6c.pid ); \
	  sleep 0.4; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | ./`basename $(Q6_CLIENT)` localhost $(PORT_Q6) >/dev/null 2>&1 || true; \
	  kill `cat .q6c.pid` 2>/dev/null || true; rm -f .q6c.pid; \
	else echo "(!) Skipping Q6 coverage run: missing binaries"; fi

	@if [ -x "$(Q7_SERVER)" ] && [ -x "$(Q7_CLIENT)" ]; then \
	  cd $(Q7_DIR) && ( ./`basename $(Q7_SERVER)` $(PORT_Q7) & echo $$! > .q7c.pid ); \
	  sleep 0.4; \
	  echo -e "euler 3 3 0-1 1-2 2-0\nquit\n" | ./`basename $(Q7_CLIENT)` localhost $(PORT_Q7) >/dev/null 2>&1 || true; \
	  kill `cat .q7c.pid` 2>/dev/null || true; rm -f .q7c.pid; \
	else echo "(!) Skipping Q7 coverage run: missing binaries"; fi

	@if [ -x "$(Q8_SERVER)" ] && [ -x "$(Q8_CLIENT)" ]; then \
	  cd $(Q8_DIR) && ( ./`basename $(Q8_SERVER)` $(PORT_Q8) & echo $$! > .q8c.pid ); \
	  sleep 0.4; \
	  echo -e "help\n3 3 0-1 1-2 2-0\nstats\nquit\n" | ./`basename $(Q8_CLIENT)` localhost $(PORT_Q8) >/dev/null 2>&1 || true; \
	  kill `cat .q8c.pid` 2>/dev/null || true; rm -f .q8c.pid; \
	else echo "(!) Skipping Q8 coverage run: missing binaries"; fi

	@if [ -x "$(Q9_SERVER)" ] && [ -x "$(Q9_CLIENT)" ]; then \
	  cd $(Q9_DIR) && ( ./`basename $(Q9_SERVER)` $(PORT_Q9) & echo $$! > .q9c.pid ); \
	  sleep 0.4; \
	  echo -e "3 3 0-1 1-2 2-0\nquit\n" | ./`basename $(Q9_CLIENT)` localhost $(PORT_Q9) >/dev/null 2>&1 || true; \
	  kill `cat .q9c.pid` 2>/dev/null || true; rm -f .q9c.pid; \
	else echo "(!) Skipping Q9 coverage run: missing binaries"; fi

	@if command -v lcov >/dev/null 2>&1; then \
	  echo "==> Collecting with lcov"; \
	  lcov --directory .. --capture --output-file $(COV_DIR)/coverage.info >/dev/null 2>&1 || true; \
	  lcov --remove $(COV_DIR)/coverage.info '/usr/*' '*/.cache/*' '*/usr/include/*' -o $(COV_DIR)/coverage.info >/dev/null 2>&1 || true; \
	else \
	  echo "==> lcov not found; generating gcov text reports"; \
	  (cd .. && gcov -o . `find . -name '*.cpp' -o -name '*.c'` >/dev/null 2>&1 || true); \
	fi
	@echo "==> Coverage data collected."

coverage-rebuild:
	@echo "==> Rebuilding all WITH coverage flags"
	@$(MAKE) -C $(Q1_DIR) clean || true; $(MAKE) -C $(Q1_DIR) CXXFLAGS+=" $(COVERAGE_CXXFLAGS)"
	@$(MAKE) -C $(Q6_DIR) clean || true; $(MAKE) -C $(Q6_DIR) CXXFLAGS+=" $(COVERAGE_CXXFLAGS)"
	@$(MAKE) -C $(Q7_DIR) clean || true; $(MAKE) -C $(Q7_DIR) CXXFLAGS+=" $(COVERAGE_CXXFLAGS)"
	@$(MAKE) -C $(Q8_DIR) clean || true; $(MAKE) -C $(Q8_DIR) CXXFLAGS+=" $(COVERAGE_CXXFLAGS)"
	# Q9: build manually with includes so Graph.h is found
	@$(MAKE) -C $(Q9_DIR) clean || true
	@echo "==> Manual build Q9 with coverage (to preserve -I includes)"
	@cd $(Q9_DIR) && \
	  $(CXX) $(Q9_CXXFLAGS) $(Q9_INCLUDES) $(Q9_SRCS_SRV) -o pipeline_server && \
	  $(CXX) $(Q9_CXXFLAGS) $(Q9_INCLUDES) $(Q9_SRCS_CLI) -o pipeline_client

coverage-html:
	@if command -v genhtml >/dev/null 2>&1 && [ -f "$(COV_DIR)/coverage.info" ]; then \
	  echo "==> Generating HTML report"; \
	  genhtml -o $(COV_HTML) $(COV_DIR)/coverage.info >/dev/null 2>&1 || true; \
	  echo "Open: $(COV_HTML)/index.html"; \
	else \
	  echo "(!) genhtml or coverage.info not found; skipping HTML generation."; \
	fi

# ======================== Utils =============================
pre_coverage_clean:
	@echo "==> Cleaning old gcov artifacts"
	@find .. -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" -delete 2>/dev/null || true

kill-servers:
	-@pkill -f "euler_server|algorithms_server|lf_server|pipeline_server" 2>/dev/null || true

preflight: prep kill-servers pre_coverage_clean

clean: kill-servers
	@rm -rf $(REPORT_DIR)
	@find .. -name "*.gcno" -o -name "*.gcda" -o -name "*.gcov" -delete 2>/dev/null || true
