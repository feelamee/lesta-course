project(test)

add_executable(
  ${PROJECT_NAME}
  main.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_library(FIND_RESULT nano-engine)
target_link_libraries(${PROJECT_NAME} PRIVATE nano-engine)
