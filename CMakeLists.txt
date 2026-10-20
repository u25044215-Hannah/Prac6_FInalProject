cmake_minimum_required(VERSION 3.10)
project(TIVIDY CXX)

# The course requires C++11. Extensions are off, so non-standard code is caught.
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

# ----core engine --------------------------------------------------
set(CORE_SOURCES
  WorkflowData.cpp
  WorkflowOutcome.cpp
  WorkflowDefinition.cpp
  InMemoryDefinitionRepository.cpp
  WorkflowInstance.cpp
  InstanceManager.cpp
  VersionManager.cpp
  TividyEngine.cpp)

# ---- Placeholders for other members' classes (see README.txt)
# Delete each file from this list when the real class is merged.
set(PLACEHOLDER_SOURCES
  ActionHistory.cpp
  CommandProcessor.cpp
  RecoveryManager.cpp)

add_library(tividy_core STATIC ${CORE_SOURCES} ${PLACEHOLDER_SOURCES})
target_include_directories(tividy_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})