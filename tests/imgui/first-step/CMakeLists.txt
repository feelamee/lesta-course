project(imgui-first-step)

add_executable(
  ${PROJECT_NAME}
  main.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC -g)

target_link_libraries(${PROJECT_NAME} PRIVATE nano)
target_link_libraries(${PROJECT_NAME} PRIVATE sanitizer)
