project(hello-world-lib)


add_library(
    hello_a STATIC
    include/hello.hpp
    src/hello.cpp
)
target_include_directories(hello_a PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(
    hello_so SHARED
    include/hello.hpp
    src/hello.cpp
)
target_include_directories(hello_so PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
set_target_properties(hello_so PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
