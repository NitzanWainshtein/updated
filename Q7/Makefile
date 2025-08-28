CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O2 -pthread
INCLUDES = -I../Q1toQ4

# Targets
TARGET_SERVER = algorithms_server
TARGET_CLIENT = algorithms_client

# Sources - adjust to actual file names
SERVER_SOURCES = server_main.cpp ../Q1toQ4/Graph.cpp \
                AlgorithmFactory.cpp EulerAlgorithm.cpp \
                Max_clique_algorithm.cpp Mst_algorithm.cpp hamilton_algorithm.cpp

CLIENT_SOURCES = client.cpp

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $(TARGET_SERVER)

$(TARGET_CLIENT): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $(CLIENT_SOURCES) -o $(TARGET_CLIENT)

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

test: all
	@echo "Q7: Multi-Algorithms Server (Strategy/Factory Patterns)"
	@echo "Terminal 1: ./$(TARGET_SERVER) [port]"
	@echo "Terminal 2: ./$(TARGET_CLIENT) [host] [port]"

run-server: $(TARGET_SERVER)
	./$(TARGET_SERVER)

run-client: $(TARGET_CLIENT)
	./$(TARGET_CLIENT)

.PHONY: all clean test run-server run-client
