project (Test_SSMS)

enable_testing()
set(GTEST_ROOTDIR "$ENV{gtestPATH}")
set(CMAKE_LIBRARY_PATH ${GTEST_ROOTDIR}/lib)
set(CMAKE_INCLUDE_PATH ${GTEST_ROOTDIR}/include)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

### Cli ######################################################################
add_executable(Test_Cli
    Test_Cli.cpp
    #../Cli.cpp
    )
target_link_libraries(Test_Cli
    ${GTEST_BOTH_LIBRARIES}
    )
add_test(Test_Cli Test_Cli)

### Crypto ###################################################################
add_executable(Test_Crypto
    Test_Crypto.cpp
    ../Crypto.cpp
    )
target_link_libraries(Test_Crypto
    ${GTEST_BOTH_LIBRARIES} pthread crypto crypt
    )
add_test(Test_Crypto Test_Crypto)

### User #####################################################################
add_executable(Test_User
    Test_User.cpp
    ../User.cpp
    ../MailBox.cpp
    #mocks/MailBox_mock.cpp
    ../Crypto.cpp
    #mocks/Crypto_mock.cpp
    )
target_link_libraries(Test_User
    ${GTEST_BOTH_LIBRARIES} pthread crypto crypt
    )
add_test(Test_User Test_User)

### MailBox ##################################################################
add_executable(Test_MailBox
    Test_MailBox.cpp
    ../MailBox.cpp
    ../User.cpp
    #mocks/User_mock.cpp
    ../Crypto.cpp
    #mocks/Crypto_mock.cpp
    )
target_link_libraries(Test_MailBox
    ${GTEST_BOTH_LIBRARIES} pthread crypto crypt
    )
add_test(Test_MailBox Test_MailBox)

##############################################################################
add_custom_target(test DEPENDS
    Test_Cli
    Test_Crypto
    Test_User
    Test_MailBox
    COMMAND ctest -V
    )