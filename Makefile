CXX = g++
CXXFLAGS = -std=c++11 -g -pthread
SRC = $(wildcard *.cpp src/hash/*.cpp src/interface/*.cpp src/quad/*.cpp src/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXECUTABLE = GIS.exe

# Default rule
all: $(EXECUTABLE)

# Rule for making the executable
$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule for making the object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.INTERMEDIATE: $(OBJ)

# Clean rule
clean:
	rm -rf $(EXECUTABLE)

.PHONY: all clean
