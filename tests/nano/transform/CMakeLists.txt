project(transform-test)

add_executable(
  ${PROJECT_NAME}
  main.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC -g)

find_library(FIND_RESULT nano)
target_link_libraries(${PROJECT_NAME} PRIVATE nano)

option(ADDRESS_SANITIZER ON)

if(ADDRESS_SANITIZER)
    message(STATUS "Using address sanitizer for " ${PROJECT_NAME})
    add_library(sanitizer INTERFACE)
    target_compile_options(sanitizer INTERFACE -fsanitize=address)
    target_link_options(sanitizer INTERFACE -fsanitize=address)

    target_link_libraries(${PROJECT_NAME} PRIVATE sanitizer)
endif()
