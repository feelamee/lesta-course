project(1-render-primitives)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/04-0-output-images)

set(SRC
    image_loader.hpp
    canvas.hpp
    color.hpp
    vertex.cpp
    vertex.hpp
    render.hpp
    line_render.hpp
    line_render.cpp
    triangle_render.hpp
    triangle_render.cpp
    shader.hpp
    shader.cpp
)
add_executable(
    main
    main.cpp
    ${SRC}
)
target_compile_features(main PUBLIC cxx_std_20)
# target_compile_options(main PUBLIC "-g3")

add_executable(
    incredible_shader
    incredible_shader.cpp
    ${SRC}
)
target_compile_features(incredible_shader PUBLIC cxx_std_20)
target_compile_options(incredible_shader PUBLIC "-O3")
find_package(SDL3 REQUIRED)
target_link_libraries(incredible_shader PUBLIC SDL3-shared)


find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Error: find_package(...) did not found boost libs")
endif()
