project(hello-world-hw)

add_subdirectory(lib)

add_executable(
    hello-world-static
    src/main.cpp
)

add_executable(
    hello-world-shared
    src/main.cpp
)

target_link_libraries(hello-world-static LINK_PUBLIC hello_a)

target_link_libraries(hello-world-shared LINK_PUBLIC hello_so)

if (WIN32)
set_target_properties(hello-world-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
