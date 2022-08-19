cmake_minimum_required(VERSION 3.16.3)
project(led_cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(depedencies/json)

find_library(GPIOD_LIBRARY gpiodcxx REQUIRED)
find_package(Threads REQUIRED)

add_executable(led_cube main.cpp)
target_link_libraries(led_cube ${GPIOD_LIBRARY} nlohmann_json::nlohmann_json Threads::Threads stdc++fs)

add_executable(filesystem fs.cpp)
target_link_libraries(filesystem ${GPIOD_LIBRARY} stdc++fs)
