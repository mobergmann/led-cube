cmake_minimum_required(VERSION 3.16.3)
project(led_cube VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(depedencies/json)

# find_library(GPIOD_LIBRARY gpiodcxx REQUIRED)
find_package(Threads REQUIRED)

set(GPIOD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/depedencies/libgpiod)
set(GPIOD_BIN ${CMAKE_CURRENT_BINARY_DIR}/depedencies/libgpiod)
set(GPIOD_STATIC_LIB ${CMAKE_CURRENT_BINARY_DIR}/depedencies/libgpiod/lib/libgpiodcxx.a)
set(GPIOD_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/depedencies/libgpiod/include)
ExternalProject_Add(libgpiod
        PREFIX ${GPIOD_BIN}
        SOURCE_DIR ${GPIOD_DIR}
        CONFIGURE_COMMAND ${GPIOD_DIR}/autogen.sh --enable-tools=no --prefix=${GPIOD_BIN} --enable-bindings-cxx
        BUILD_COMMAND make && make install
        INSTALL_COMMAND make install)

set(PROJECT_SOURCES
        src/FileTransfer.cpp include/FileTransfer.h include/Frame.h include/Button.h main.cpp)

add_executable(led_cube ${PROJECT_SOURCES})

add_dependencies(led_cube libgpiod)

set_target_properties(led_cube PROPERTIES IMPORTED_LOCATION ${GPIOD_STATIC_LIB})
set_target_properties(led_cube PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GPIOD_INCLUDES})

target_link_libraries(led_cube PRIVATE 
        Threads::Threads 
        stdc++fs
        nlohmann_json::nlohmann_json
        # libgpiod
        # ${GPIOD_LIBRARY}
        )

# install(TARGETS led_cube
#     DESTINATION led-cube)

# set(CPACK_PACKAGE_NAME "led-cube")
# # set(CPACK_PACKAGE_VENDOR "mobergmann")
# set(CPACK_PACKAGE_CONTACT "mobergmann@pm.me")
# include(CPack)
