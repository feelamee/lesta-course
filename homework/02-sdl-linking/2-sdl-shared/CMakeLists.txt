project(sdl-version-shared)

set(TARGET sdl-version-shared)
set(SRC_DIR src)
add_executable(
    ${TARGET}
    ${SRC_DIR}/main.cpp
)

target_compile_features(${TARGET} PRIVATE cxx_std_20)
target_compile_options(${TARGET} PRIVATE "-g")
target_include_directories(${TARGET} PRIVATE ${SDL3_INCLUDE_DIR})
target_link_libraries(${TARGET} PRIVATE SDL3::SDL3-shared)
