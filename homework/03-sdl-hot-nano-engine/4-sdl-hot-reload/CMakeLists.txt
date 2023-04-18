project(4-sdl-hot-reload)

set(TARGET_EXE 4-nano-engine)
add_executable(
    ${TARGET_EXE}
    engine/src/engine.cpp
    engine/src/engine.hpp
    engine/src/event.cpp
    engine/include/iengine.hpp
    engine/include/event.hpp
)
set_target_properties(
   ${TARGET_EXE} PROPERTIES
   ENABLE_EXPORTS 2
)

target_include_directories(${TARGET_EXE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine/include)
target_compile_features(${TARGET_EXE} PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)
target_link_libraries(${TARGET_EXE} PUBLIC SDL3-shared)

set(TARGET_LIB 4-game)
add_library(
    ${TARGET_LIB} SHARED
    game/src/main.cpp
)
target_include_directories(${TARGET_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine/include)
target_compile_features(${TARGET_LIB} PUBLIC cxx_std_20)
