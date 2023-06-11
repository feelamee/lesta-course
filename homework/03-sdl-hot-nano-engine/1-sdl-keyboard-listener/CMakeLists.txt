project(1-sdl-keyboard-listener)

add_library(
    1-nano-engine SHARED
    engine/src/engine.cpp
    engine/src/event.cpp
    engine/include/engine.hpp
    engine/include/event.hpp
)
target_include_directories(1-nano-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine/include)
target_compile_features(1-nano-engine PUBLIC cxx_std_20)

target_link_libraries(1-nano-engine PUBLIC SDL3::SDL3-shared)

add_executable(
    1-game 
    game/src/main.cpp
)
target_compile_features(1-game PUBLIC cxx_std_20)
target_link_libraries(1-game PUBLIC 1-nano-engine)
