project(tetris)

set(target ${PROJECT_NAME})

add_executable(
  ${target}
  main.cpp
)
target_link_libraries(${target} PUBLIC nano)
target_link_libraries(${target} PUBLIC sanitizer)
