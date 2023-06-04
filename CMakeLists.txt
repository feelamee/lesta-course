cmake_minimum_required(VERSION 3.26)
project(lesta-course)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/homework)

add_subdirectory(${CMAKE_SOURCE_DIR}/external)

add_subdirectory(${CMAKE_SOURCE_DIR}/nano)

add_subdirectory(${CMAKE_SOURCE_DIR}/tests)

option(ADDRESS_SANITIZER ON)

if(ADDRESS_SANITIZER)
    message(STATUS "Using address sanitizer for " ${PROJECT_NAME})
    add_library(sanitizer INTERFACE)
    target_compile_options(sanitizer INTERFACE -fsanitize=address)
    target_link_options(sanitizer INTERFACE -fsanitize=address)
endif()
