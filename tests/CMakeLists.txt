cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(test_wave_shell CXX)


file(GLOB_RECURSE SOURCES "../src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "main.cpp$")
file(GLOB TEST_SOURCES "*.cpp") 

set(GTEST_DIR ./googletest)  
add_subdirectory(${GTEST_DIR} gtest)

add_executable(wsh_test ${SOURCES} ${TEST_SOURCES})

include_directories(../include) 

target_link_libraries(wsh_test PRIVATE 
    gtest
    gtest_main
)

target_compile_options(wsh_test PRIVATE -Wall -Wextra -g) 

set(CMAKE_BUILD_TYPE Debug)




