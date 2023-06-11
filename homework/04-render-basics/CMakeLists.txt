project(04-render-basics)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/04-0-output-images)

set(TARGET software-render)
add_library(
    ${TARGET} STATIC
    src/line_render.cpp
    src/triangle_render.cpp
    src/shader.cpp
    src/canvas.cpp
    src/resource_loader.cpp
)
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${TARGET} PUBLIC cxx_std_20)
target_compile_options(${TARGET} PUBLIC "-O3")

target_link_libraries(${TARGET} PUBLIC SDL3::SDL3-shared)

add_executable(
  main
  main.cpp
)
target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE software-render)

add_executable(
  incredible-shader
  incredible-shader.cpp
)
target_compile_features(incredible-shader PRIVATE cxx_std_20)
target_link_libraries(incredible-shader PRIVATE software-render)
