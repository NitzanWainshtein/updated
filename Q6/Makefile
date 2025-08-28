CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O2 -pthread
INCLUDES = -I../Q1toQ4

# Targets
TARGET_SERVER = euler_server
TARGET_CLIENT = euler_client

# Sources  
SERVER_SOURCES = server_main.cpp ../Q1toQ4/Graph.cpp
CLIENT_SOURCES = client.cpp

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $(TARGET_SERVER)

$(TARGET_CLIENT): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $(CLIENT_SOURCES) -o $(TARGET_CLIENT)

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

test: all
	@echo "Q6: Euler Circuit Server"
	@echo "Terminal 1: ./$(TARGET_SERVER) [port]"
	@echo "Terminal 2: ./$(TARGET_CLIENT) [host] [port]"
	@echo "Or telnet: telnet localhost 8080"

run-server: $(TARGET_SERVER)
	./$(TARGET_SERVER)

run-client: $(TARGET_CLIENT)
	./$(TARGET_CLIENT)

.PHONY: all clean test run-server run-client