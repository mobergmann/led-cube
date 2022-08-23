cmake_minimum_required(VERSION 3.16.3)
project(led_cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(depedencies/json)

find_library(GPIOD_LIBRARY gpiodcxx REQUIRED)
find_package(Threads REQUIRED)

set(PROJECT_SOURCES
    Frame.h Button.h main.cpp Frame.h)

add_executable(led_cube ${PROJECT_SOURCES})
target_link_libraries(led_cube ${GPIOD_LIBRARY} nlohmann_json::nlohmann_json Threads::Threads stdc++fs)

add_executable(test test.cpp)
target_link_libraries(test ${GPIOD_LIBRARY} nlohmann_json::nlohmann_json Threads::Threads stdc++fs)
