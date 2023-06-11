project(1-hopeful-sky)

find_package(OpenGL REQUIRED)

set(target ${PROJECT_NAME})
add_executable(
  ${target}
  main.cpp
)
# target_compile_options(${target} PUBLIC "-g3")
target_compile_features(${target} PRIVATE cxx_std_20)
target_link_libraries(${target} PUBLIC OpenGL::GL)
target_link_libraries(${target} PUBLIC SDL3::SDL3-shared)
target_link_libraries(${target} PUBLIC glad)
