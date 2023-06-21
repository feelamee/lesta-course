cmake_minimum_required(VERSION 3.22)
project(tetris)

option(ADDRESS_SANITIZER ON)

add_library(sanitizer INTERFACE)

if(ADDRESS_SANITIZER)
    message(STATUS "Using address sanitizer")
    target_compile_options(sanitizer INTERFACE -fsanitize=address)
    target_link_options(sanitizer INTERFACE -fsanitize=address)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nano)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external)

set(target ${PROJECT_NAME})

if (ANDROID)
add_library(
  ${target} SHARED
  main.cpp
)
else()
add_executable(
    ${target}
    main.cpp
)
endif()

target_link_libraries(${target} PUBLIC nano)
target_link_libraries(${target} PUBLIC sanitizer)
