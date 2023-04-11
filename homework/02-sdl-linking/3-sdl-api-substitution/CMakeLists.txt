project(sdl-api-substitution)

set(LOCAL_INCLUDE_DIR $ENV{HOME}/.local/include)
set(SDL2_LIB $ENV{HOME}/.local/lib/libSDL2.a)

set(TARGET sdl-api-substitution)
set(SRC_DIR src)
add_executable(
    ${TARGET}
    ${SRC_DIR}/main.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_20)
#target_compile_options(${TARGET} PRIVATE "-g")

target_include_directories(${TARGET} PRIVATE ${LOCAL_INCLUDE_DIR})
target_link_libraries(${TARGET} PRIVATE ${SDL2_LIB})
