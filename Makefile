# Variables
CXX = g++
CXXFLAGS = -std=c++11 -g -pthread
SRC = $(wildcard *.cpp src/hash/*.cpp src/interface/*.cpp src/quad/*.cpp src/*.cpp)
EXECUTABLE = GIS.exe

# Default rule
all: $(EXECUTABLE)

# Rule for making the executable
$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXECUTABLE)

# Clean rule
clean:
	rm -rf $(EXECUTABLE)

.PHONY: all clean
