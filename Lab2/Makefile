CXX     = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGETS = 1/hierarquia \
          2/cmd \
          3/vector \
          4/shell

all: $(TARGETS)

1/hierarquia: 1/hierarquia.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

2/cmd: 2/cmd.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

3/vector: 3/vector.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

4/shell: 4/shell.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean