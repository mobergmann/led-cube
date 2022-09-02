cmake_minimum_required(VERSION 3.16.3)
project(led_cube VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(dependencies/json)

# find_library(GPIOD_LIBRARY gpiodcxx REQUIRED)
find_package(Threads REQUIRED)


#region setup self build libgpiod
set(GPIOD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libgpiod)
set(GPIOD_BIN ${CMAKE_CURRENT_BINARY_DIR}/dependencies/libgpiod)
set(GPIOD_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependencies/libgpiod/lib)
set(GPIOD_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependencies/libgpiod/include)
ExternalProject_Add(libgpiod-project
        PREFIX ${GPIOD_BIN}
        SOURCE_DIR ${GPIOD_DIR}
        CONFIGURE_COMMAND ${GPIOD_DIR}/autogen.sh --enable-tools=no --prefix=${GPIOD_BIN} --enable-bindings-cxx
        BUILD_COMMAND make && make install
        INSTALL_COMMAND make install
        BUILD_BYPRODUCTS ${GPIOD_LIB_DIR}/libgpiodcxx.so)

add_library(libgpiod SHARED IMPORTED) # todo make to STATIC
set_target_properties(libgpiod PROPERTIES IMPORTED_LOCATION ${GPIOD_LIB_DIR}/libgpiodcxx.so)
#endregion

add_executable(led_cube)

target_sources(${PROJECT_NAME} PRIVATE
    src/FileTransfer.cpp include/FileTransfer.h
    include/Button.h
    include/Frame.h
    main.cpp)

#target_link_directories(led_cube PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/dependencies/libgpiod/lib)

target_link_libraries(led_cube PRIVATE
    Threads::Threads
    stdc++fs
    nlohmann_json::nlohmann_json
    libgpiod)

target_compile_features(${PROJECT_NAME} PRIVATE
    cxx_std_20)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wno-unknown-pragmas
    -Werror)

add_dependencies(led_cube libgpiod)

#set_target_properties(led_cube PROPERTIES IMPORTED_LOCATION ${GPIOD_STATIC_LIB})
#set_target_properties(led_cube PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GPIOD_INCLUDES})

# install(TARGETS led_cube
#     DESTINATION led-cube)

# set(CPACK_PACKAGE_NAME "led-cube")
# # set(CPACK_PACKAGE_VENDOR "mobergmann")
# set(CPACK_PACKAGE_CONTACT "mobergmann@pm.me")
# include(CPack)
