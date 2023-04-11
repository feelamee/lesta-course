project(sdl-api-substitution)

set(LOCAL_INCLUDE_DIR $ENV{HOME}/.local/include)
set(SDL2_LIB $ENV{HOME}/.local/lib/libSDL2.a)

if(NOT EXISTS ${LOCAL_INCLUDE_DIR}/SDL2 OR NOT EXISTS ${SDL2_LIB})
    message(FATAL_ERROR "sdl2 include dir doesn't exist in ~/.local/include"
                  " or libSDL2.a doesn't exist")
endif()

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
