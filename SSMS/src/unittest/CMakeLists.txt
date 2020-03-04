project (Test_SSMS)

enable_testing()
set(GTEST_ROOTDIR "$ENV{gtestPATH}")
set(CMAKE_LIBRARY_PATH ${GTEST_ROOTDIR}/lib)
set(CMAKE_INCLUDE_PATH ${GTEST_ROOTDIR}/include)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Test_ssms Test_Ssms.cpp)
target_link_libraries(Test_ssms
    ${GTEST_BOTH_LIBRARIES} pthread crypto crypt
    )

add_test(Test_ssms Test_ssms)

add_custom_target(test 
                   DEPENDS Test_ssms
                   COMMAND ctest -V )


