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

set(SDL3IMAGE_AVIF OFF)
set(SDL3IMAGE_JPG OFF)
set(SDL3IMAGE_JPG_SAVE OFF)
set(SDL3IMAGE_BMP OFF)
set(SDL3IMAGE_GIF OFF)
set(SDL3IMAGE_LBM OFF)
set(SDL3IMAGE_PCX OFF)
set(SDL3IMAGE_PNG_SAVE OFF)
set(SDL3IMAGE_PNM OFF)
set(SDL3IMAGE_QOI OFF)
set(SDL3IMAGE_SVG OFF)
set(SDL3IMAGE_SVG OFF)
set(SDL3IMAGE_TIF OFF)
set(SDL3IMAGE_WEBP OFF)
set(SDL3IMAGE_XCF OFF)
set(SDL3IMAGE_XPM OFF)
set(SDL3IMAGE_XV OFF)
set(SDL3IMAGE_PNG ON)

set(BUILD_SDL3IMAGE ON)

set(SDL3_TESTS OFF)
set(BUILD_SDL3 ON)

set(BUILD_GLAD ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nano)


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
