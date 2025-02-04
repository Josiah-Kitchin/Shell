cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(wave_shell CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# library for json parsing 
set(JSON_DIR ./libs/json)
add_subdirectory(${JSON_DIR} nlohmann_json)


add_executable(wsh ${SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


target_link_libraries(wsh PRIVATE
    nlohmann_json
)


target_compile_options(wsh PRIVATE -Wall -Wextra -g)
