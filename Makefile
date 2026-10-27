CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g

# Compile every .cpp in the current folder except the main application entry point.
# Put any .cpp files you are NOT ready to compile in another folder until merged.
SOURCES := $(filter-out main.cpp mini_main.cpp,$(wildcard *.cpp))
OBJECTS := $(patsubst %.cpp,build/%.o,$(SOURCES))
TEST_OBJECT := build/mini_main.o
TEST_BINARY := build/mini_test

.PHONY: all test clean
all: $(TEST_BINARY)

$(TEST_BINARY): $(TEST_OBJECT) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: %.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_BINARY)
	./$(TEST_BINARY)

clean:
	rm -rf build
