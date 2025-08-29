CXX := g++
CXXFLAGS := -std=c++17 -O2 -pthread -Wall -Wextra -I../Q1toQ4 -I../Q7 -I.

SRV_SRCS := server_main.cpp PipelineServer.cpp ../Q1toQ4/Graph.cpp \
            ../Q7/AlgorithmFactory.cpp ../Q7/EulerAlgorithm.cpp \
            ../Q7/Max_clique_algorithm.cpp ../Q7/Mst_algorithm.cpp ../Q7/hamilton_algorithm.cpp

CLI_SRCS := client.cpp

all: pipeline_server pipeline_client

pipeline_server: $(SRV_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

pipeline_client: $(CLI_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f pipeline_server pipeline_client *.o
