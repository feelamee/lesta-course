project(tests)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nano
                 ${CMAKE_CURRENT_BINARY_DIR}/nano)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui
                 ${CMAKE_CURRENT_BINARY_DIR}/imgui)
