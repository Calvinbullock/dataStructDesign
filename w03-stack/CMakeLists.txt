cmake_minimum_required(VERSION 3.5)
project(runMe)

# Include directories for header files
include_directories(src)

# Set source files
set(SOURCE_FILES ./testStack.cpp)

# Generate executable
add_executable(runMe ${SOURCE_FILES})
