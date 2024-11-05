# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -O3 -Wall -std=c++11
# Target executable name
TARGET = sin_lut

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp -lm

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
