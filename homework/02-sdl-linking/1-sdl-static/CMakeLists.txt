cmake_minimum_required(VERSION 3.26)
project(sdl-version-static)

set(SDL3_INCLUDE_DIR $ENV{HOME}/.local/include)
set(SDL3_STATIC_LIB $ENV{HOME}/.local/lib/libSDL3.a)

set(TARGET sdl-version-static)
set(SRC_DIR src)
add_executable(
    ${TARGET}
    ${SRC_DIR}/main.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_20)
target_include_directories(${TARGET} PRIVATE ${SDL3_INCLUDE_DIR})
target_link_libraries(${TARGET} PRIVATE ${SDL3_STATIC_LIB})
