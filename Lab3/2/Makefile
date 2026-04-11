CC=gcc
CXX=g++
CFLAGS=-I.
CXXFLAGS=-I. -pthread
DEPS = matriz.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matrix: matrix.cpp matriz.c matriz.h
	$(CXX) -o matrix matrix.cpp matriz.c $(CXXFLAGS)

matriz: matriz.o test_matriz.o 
	$(CC) -o test_matriz matriz.o test_matriz.o

clean: 
	rm -f *.o test_matriz matrix
