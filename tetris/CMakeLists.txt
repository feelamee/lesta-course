cmake_minimum_required(VERSION 3.22)
project(tetris)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

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
)
else()
add_executable(
    ${target}
  )
endif()

target_sources(${target} PUBLIC
  src/main.cpp
  src/menu_scene.cpp
  src/game_scene.cpp
  src/tetramino.cpp
)

target_link_libraries(${target} PUBLIC nano)
target_link_libraries(${target} PUBLIC sanitizer)
