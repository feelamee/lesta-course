project(2-in-search-of-sdl-static)

set(TARGET_LIB 2-nano-engine)
add_library(
    ${TARGET_LIB} STATIC
    engine/src/engine.cpp
    engine/src/event.cpp
    engine/include/engine.hpp
    engine/include/event.hpp
)
target_include_directories(${TARGET_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine/include)
target_compile_features(${TARGET_LIB} PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)
target_link_libraries(${TARGET_LIB} PUBLIC SDL3-shared)

set(TARGET_EXE 2-game)
add_executable(
    ${TARGET_EXE}
    game/src/main.cpp
)
target_compile_features(${TARGET_EXE} PUBLIC cxx_std_20)
target_link_libraries(${TARGET_EXE} PUBLIC ${TARGET_LIB})