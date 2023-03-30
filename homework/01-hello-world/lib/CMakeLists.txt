project(hello-world-lib)


add_library(
    libhello-static STATIC
    include/hello.hpp
    src/hello.cpp
)

add_library(
    libhello-shared SHARED
    include/hello.hpp
    src/hello.cpp
)

target_include_directories(libhello-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(libhello-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)