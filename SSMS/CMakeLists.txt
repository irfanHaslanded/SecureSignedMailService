cmake_minimum_required (VERSION 2.6)

# Use C++ 11
set (CMAKE_CXX_STANDARD 11)

# Force the use of gcc and g++ and not cc which might be older
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
find_program(CMAKE_MAKE_PROGRAM
  NAMES make)
  
project (SSMS)
add_subdirectory(src)
