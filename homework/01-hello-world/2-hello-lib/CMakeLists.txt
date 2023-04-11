project(hello-lib CXX)

add_subdirectory(lib)

add_executable(
    hello-world-static
    src/main.cpp
)
target_compile_features(hello-world-static PRIVATE cxx_std_20)
target_link_libraries(hello-world-static LINK_PUBLIC hello_a)

add_executable(
    hello-world-shared
    src/main.cpp
)
target_compile_features(hello-world-shared PRIVATE cxx_std_20)
target_link_libraries(hello-world-shared LINK_PUBLIC hello_so)

if (WIN32)
set_target_properties(hello-world-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
