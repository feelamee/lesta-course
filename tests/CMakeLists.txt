project(tests)

execute_process (
    COMMAND bash -c "git submodule update --init ../external/Catch2"
    OUTPUT_VARIABLE INIT_SUBMODULES_CATCH2_RESULT
)
set(BUILD_CATCH2 ON)
set(SDL_TESTS ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/external ${CMAKE_CURRENT_BINARY_DIR}/lib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nano
                 ${CMAKE_CURRENT_BINARY_DIR}/nano)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui
                 ${CMAKE_CURRENT_BINARY_DIR}/imgui)
