project(sdl-version-static)

set(SDL3_INCLUDE_DIR $ENV{HOME}/.local/include)
set(SDL3_LIB $ENV{HOME}/.local/lib/libSDL3.a)

if(NOT EXISTS ${SDL3_INCLUDE_DIR}/SDL3 OR NOT EXISTS ${SDL3_LIB})
    message(FATAL_ERROR "sdl3 include dir doesn't exist in ~/.local/include"
                  " or libSDL3.a doesn't exist")
endif()

set(TARGET sdl-version-static)
set(SRC_DIR src)
add_executable(
    ${TARGET}
    ${SRC_DIR}/main.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_20)
target_include_directories(${TARGET} PRIVATE ${SDL3_INCLUDE_DIR})
target_link_libraries(${TARGET} PRIVATE ${SDL3_LIB})
