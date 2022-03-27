cmake_minimum_required(VERSION 2.6)
 
# Locate GTest

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories (${TEST_SOURCE_DIR}/src)
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp)
target_link_libraries(runTests Game ${GTEST_LIBRARIES} pthread)