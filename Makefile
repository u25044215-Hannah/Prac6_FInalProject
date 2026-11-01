

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g -I. -Iinclude

.PHONY: all run test demo gui clean


# Build the main C++ integration executable
all:
	$(MAKE) -f Makefile.integration


# Run the interactive C++ workflow demonstration
run: all
	./whole_project_demo


# Run all 60 existing integration tests
test:
	$(MAKE) -f Makefile.integration test


# Build and run the C++ demonstration
demo: all
	./whole_project_demo


# Build the C++ executable and launch the GUI
gui: all
	python3 COS214_GUI/gui_server.py



# Remove generated integration executable
clean:
	rm -f whole_project_demo
