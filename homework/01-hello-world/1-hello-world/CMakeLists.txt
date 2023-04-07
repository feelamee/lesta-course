project(hello-world CXX)
set(CMAKE_CXX_STANDARD 20)

set(SRC_DIR src)
add_executable(
    hello-world
    ${SRC_DIR}/main.cpp
)
