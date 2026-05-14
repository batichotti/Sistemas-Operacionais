CC := gcc
CXX := g++

CFLAGS := -Wall -Wextra -pthread
CXXFLAGS := -Wall -Wextra -pthread

TARGETS := ex1 ex2

.PHONY: all clean

all: $(TARGETS)

ex1: ex1.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ex2: ex2.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
