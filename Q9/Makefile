CXX := g++
CXXFLAGS := -std=c++17 -O2 -pthread -Wall -Wextra -g
INCLUDES := -I../Q1toQ4 -I../Q7 -I.

# Check if required files exist
GRAPH_FILE := ../Q1toQ4/Graph.cpp
ALG_FACTORY := ../Q7/AlgorithmFactory.cpp
EULER_ALG := ../Q7/EulerAlgorithm.cpp
CLIQUE_ALG := ../Q7/Max_clique_algorithm.cpp
MST_ALG := ../Q7/Mst_algorithm.cpp
HAMILTON_ALG := ../Q7/hamilton_algorithm.cpp

# Source files
SERVER_SRCS := server_main.cpp PipelineServer.cpp ActiveObject.cpp \
               $(GRAPH_FILE) \
               $(ALG_FACTORY) $(EULER_ALG) \
               $(CLIQUE_ALG) $(MST_ALG) \
               $(HAMILTON_ALG)

CLIENT_SRCS := client.cpp

# Target binaries
SERVER_TARGET := pipeline_server
CLIENT_TARGET := pipeline_client

all: check-deps $(SERVER_TARGET) $(CLIENT_TARGET)

check-deps:
	@echo "Checking dependencies..."
	@test -f $(GRAPH_FILE) || (echo "Error: $(GRAPH_FILE) not found. Build Q1toQ4 first." && exit 1)
	@test -f $(ALG_FACTORY) || (echo "Error: $(ALG_FACTORY) not found. Build Q7 first." && exit 1)
	@echo "Dependencies OK."

$(SERVER_TARGET): $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

$(CLIENT_TARGET): $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET) *.o *.gcda *.gcno *.gcov

test: all
	@echo "Q9: Pipeline Server with 4 Active Objects"
	@echo ""
	@echo "Terminal 1: ./$(SERVER_TARGET) [port]"
	@echo "Terminal 2: ./$(CLIENT_TARGET) [host] [port]"
	@echo "Or telnet: telnet localhost 8083"
	@echo ""
	@echo "Pipeline Architecture:"
	@echo "  Stage 1: Request Receiver - Parse input"
	@echo "  Stage 2: Graph Processor - Validate graph"
	@echo "  Stage 3: Algorithm Executor - Run all 4 algorithms"
	@echo "  Stage 4: Response Formatter - Format and send results"
	@echo ""
	@echo "Test commands:"
	@echo "  help - Show format"
	@echo "  stats - Show pipeline statistics"
	@echo "  3 3 0-1 1-2 2-0 - Triangle graph"
	@echo "  4 4 0-1 1-2 2-3 3-0 - Square graph"
	@echo "  quit - Exit"

run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

run-client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

.PHONY: all clean test run-server run-client