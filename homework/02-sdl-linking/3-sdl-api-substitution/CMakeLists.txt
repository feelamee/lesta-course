project(sdl-api-substitution)

set(TARGET sdl-api-substitution)
set(SRC_DIR src)
add_executable(
    ${TARGET}
    ${SRC_DIR}/main.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_20)
#target_compile_options(${TARGET} PRIVATE "-g")

target_link_libraries(${TARGET} PRIVATE SDL3::SDL3-shared)
