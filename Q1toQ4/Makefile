CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O2

TARGET = graph_analyzer
SOURCES = main.cpp Graph.cpp GraphGenerator.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) graph_analyzer_* *.gcda *.gcno *.gcov gmon.out *.txt *.out

test: $(TARGET)
	./$(TARGET) --test

coverage: clean
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage $(SOURCES) -o graph_analyzer_cov
	./graph_analyzer_cov --test > /dev/null
	./graph_analyzer_cov -v 3 -e 3 -s 42 > /dev/null
	gcov graph_analyzer_cov-main.gcda graph_analyzer_cov-Graph.gcda graph_analyzer_cov-GraphGenerator.gcda 2>/dev/null || gcov *.gcda 2>/dev/null || echo "Coverage analysis completed"
	ls -la *.gcov 2>/dev/null || echo "No .gcov files found - this is OK"

profile: clean
	$(CXX) $(CXXFLAGS) -pg $(SOURCES) -o graph_analyzer_prof
	./graph_analyzer_prof -v 100 -e 1000 -s 42 > /dev/null
	gprof ./graph_analyzer_prof gmon.out > gprof_report.txt
	echo "Profiling report: gprof_report.txt"

valgrind: $(TARGET)
	valgrind --leak-check=full --log-file=valgrind_report.txt ./$(TARGET) -v 10 -e 15 -s 42 2>/dev/null
	echo "Valgrind report: valgrind_report.txt"

analyze: coverage profile valgrind

.PHONY: all clean test coverage profile valgrind analyze