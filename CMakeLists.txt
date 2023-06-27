cmake_minimum_required(VERSION 3.26)
project(lesta-course)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/external)

option(BUILD_NANO "enable building of nano engine" ON)

if (BUILD_NANO OR BUILD_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/nano)
endif()

option(BUILD_HW "enable building of homework" ON)

if (BUILD_HW OR BUILD_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/homework)
endif()

option (BUILD_TESTS "enable building of tests" ON)

if (BUILD_TESTS OR BUILD_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif()

option(ADDRESS_SANITIZER ON)

add_library(sanitizer INTERFACE)

if(ADDRESS_SANITIZER)
    message(STATUS "Using address sanitizer")
    target_compile_options(sanitizer INTERFACE -fsanitize=address)
    target_link_options(sanitizer INTERFACE -fsanitize=address)
endif()
