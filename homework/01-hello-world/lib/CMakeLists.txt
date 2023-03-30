project(hello-world-lib)


add_library(
    hello_a STATIC
    include/hello.hpp
    src/hello.cpp
)

add_library(
    hello_so SHARED
    include/hello.hpp
    src/hello.cpp
)

target_include_directories(hello_a PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(hello_so PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
