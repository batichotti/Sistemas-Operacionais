CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = ex2
SRC = ex2.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET) write dados.txt

.PHONY: all clean run
