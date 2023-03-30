project(hello-world-hw)

add_subdirectory(lib)

add_executable(
    hello-static
    src/main.cpp
)

add_executable(
    hello-shared
    src/main.cpp
)

target_link_libraries(hello-static LINK_PUBLIC libhello-static)
target_link_libraries(hello-shared LINK_PUBLIC libhello-shared)