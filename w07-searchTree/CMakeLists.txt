set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
cmake_minimum_required(VERSION 3.5)
project(runMe)

# Include directories for header files
include_directories(src)

# Set source files
set(SOURCE_FILES ./testBST.cpp)

# Generate executable
add_executable(runMe ${SOURCE_FILES})
