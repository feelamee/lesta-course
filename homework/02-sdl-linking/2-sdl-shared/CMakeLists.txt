project(sdl-version-shared)

set(SDL3_INCLUDE_DIR $ENV{HOME}/.local/include)
set(SDL3_LIB $ENV{HOME}/.local/lib/libSDL3.so)

set(TARGET sdl-version-shared)
set(SRC_DIR src)
add_executable(
    ${TARGET}
    ${SRC_DIR}/main.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_20)
target_compile_options(${TARGET} PRIVATE "-g")
target_include_directories(${TARGET} PRIVATE ${SDL3_INCLUDE_DIR})
target_link_libraries(${TARGET} PRIVATE ${SDL3_LIB})
