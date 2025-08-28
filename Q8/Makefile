CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O2 -pthread
INCLUDES = -I../Q1toQ4 -I../Q7

# Targets
TARGET_SERVER = lf_server
TARGET_CLIENT = lf_client

# Sources - reuse algorithms from Q7
SERVER_SOURCES = server_main.cpp LFServer.cpp \
                ../Q1toQ4/Graph.cpp \
                ../Q7/AlgorithmFactory.cpp \
                ../Q7/EulerAlgorithm.cpp \
                ../Q7/Max_clique_algorithm.cpp \
                ../Q7/Mst_algorithm.cpp \
                ../Q7/hamilton_algorithm.cpp

CLIENT_SOURCES = client.cpp

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $(TARGET_SERVER)

$(TARGET_CLIENT): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $(CLIENT_SOURCES) -o $(TARGET_CLIENT)

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

test: all
	@echo "Q8: Leader-Follower Threading Pattern"
	@echo ""
	@echo "Terminal 1: ./$(TARGET_SERVER) [port] [workers]"
	@echo "Terminal 2: ./$(TARGET_CLIENT) [host] [port]"  
	@echo ""
	@echo "Leader-Follower Pattern:"
	@echo "- Only ONE worker acts as Leader at a time"
	@echo "- Leader accepts requests, then promotes new Leader"
	@echo "- Former Leader processes request as Follower"
	@echo "- ALL 4 algorithms executed on each graph"
	@echo ""
	@echo "Test graphs:"
	@echo "  3 3 0-1 1-2 2-0"
	@echo "  4 4 0-1 1-2 2-3 3-0"
	@echo "  4 6 0-1 0-2 0-3 1-2 1-3 2-3"

run-server: $(TARGET_SERVER)
	./$(TARGET_SERVER)

run-client: $(TARGET_CLIENT)
	./$(TARGET_CLIENT)

.PHONY: all clean test run-server run-client