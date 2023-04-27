project(1-render-primitives)

set(SRC_GEN
    image_loader.hpp
    canvas.hpp
    color.hpp
    render.hpp
)

set(1-target 04-0-load-dump-ppm)
add_executable(
    ${1-target}
    main_canvas.cpp
    ${SRC_GEN}
)
target_compile_features(${1-target} PUBLIC cxx_std_20)
target_compile_options(${1-target} PUBLIC "-Wall")

set(2-target 04-0-line-render)
add_executable(
    ${2-target}
    main_line_render.cpp
    line_render.hpp
    ${SRC_GEN}
)
target_compile_features(${2-target} PUBLIC cxx_std_20)
target_compile_options(${2-target} PUBLIC "-Wall")


set(3-target 04-0-triangle-render)
add_executable(
    ${3-target}
    main_triangle_render.cpp
    line_render.hpp
    triangle_render.hpp
    ${SRC_GEN}
)
target_compile_features(${3-target} PUBLIC cxx_std_20)
target_compile_options(${3-target} PUBLIC "-Wall")

find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Error: find_package(...) did not found boost libs")
endif()
