project(06-and-god-create-sounds)

set(target ${PROJECT_NAME})
add_executable(
  ${target}
  main.cpp
)
target_link_libraries(${target} PUBLIC nano)
target_compile_features(${target} PRIVATE cxx_std_20)
target_link_libraries(${target} PRIVATE sanitizer)
