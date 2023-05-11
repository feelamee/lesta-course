cmake_minimum_required(VERSION 3.26)
project(lesta-course)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/homework)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL
                 ${CMAKE_BINARY_DIR}/lib/SDL)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad
                 ${CMAKE_BINARY_DIR}/lib/glad)

add_subdirectory(${CMAKE_SOURCE_DIR}/engine)
