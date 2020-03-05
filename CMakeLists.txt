project(bajs)

set(GTest_REQUIRED /app/vbuild/RHEL7-x86_64/gtest/1.8.0/)
set(CMAKE_LIBRARY_PATH /app/vbuild/RHEL7-x86_64/gtest/1.8.0/lib) 
set(CMAKE_INCLUDE_PATH /app/vbuild/RHEL7-x86_64/gtest/1.8.0/include)






find_package(GTest_REQUIRED)
