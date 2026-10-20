cmake_minimum_required(VERSION 3.10)
project(TIVIDY CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

option(TIVIDY_SANITIZE "Build with AddressSanitizer and UBSan" OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -pedantic)
  if(TIVIDY_SANITIZE)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
  endif()
endif()

# ---- core engine (add each file here in the same commit as the file)
set(CORE_SOURCES
  WorkflowData.cpp
  WorkflowOutcome.cpp)

add_library(tividy_core STATIC ${CORE_SOURCES})
target_include_directories(tividy_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ---- Tests (doctest) ----
enable_testing()
add_executable(tividy_tests
  tests/test_main.cpp
  tests/test_workflow_data.cpp
  tests/test_workflow_outcome.cpp)
target_include_directories(tividy_tests PRIVATE third_party/doctest tests)
target_link_libraries(tividy_tests tividy_core)
add_test(NAME tividy_tests COMMAND tividy_tests)