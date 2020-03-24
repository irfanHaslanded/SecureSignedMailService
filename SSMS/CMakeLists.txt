cmake_minimum_required (VERSION 2.6)

# Use C++ 11
set (CMAKE_CXX_STANDARD 11)

# Add debug symbols
set (CMAKE_BUILD_TYPE Debug)

# Generate compile commands
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Force the use of gcc and g++ and not cc which might be older
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
find_program(CMAKE_MAKE_PROGRAM
  NAMES make)
  
project (SSMS)
add_subdirectory(src)
