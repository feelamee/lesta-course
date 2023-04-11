project(hello-world CXX)

add_executable(
    hello-world
    src/main.cpp
)

target_compile_features(hello-world PRIVATE cxx_std_20)
