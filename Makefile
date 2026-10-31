CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g

SOURCES := $(filter-out WholeProject_IntegrationMain.cpp work_behaviour_test.cpp,$(wildcard *.cpp))
# Sources that live in include/ (the filename-with-space duplicates are not built).
INCLUDE_SOURCES := include/ConnectorRegistry.cpp include/WorkflowEvent.cpp include/WorkflowEventPublisher.cpp
OBJECTS := $(patsubst %.cpp,build/%.o,$(SOURCES)) $(patsubst include/%.cpp,build/include_%.o,$(INCLUDE_SOURCES))
TEST_OBJECT := build/MiniMain_Testing.o
TEST_BINARY := build/MiniMain_Testing

.PHONY: all test clean
all: $(TEST_BINARY)

$(TEST_BINARY): $(TEST_OBJECT) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: %.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/include_%.o: include/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

test: $(TEST_BINARY)
	./$(TEST_BINARY)

clean:
	rm -rf build

.PHONY: run

run: all
	./build/MiniMain_Testing

.PHONY: demo

demo:
	$(MAKE) -f Makefile.integration
	./whole_project_demo


.PHONY: gui

gui:
	$(MAKE) -f Makefile.integration
	python3 COS214_GUI/gui_server.py
