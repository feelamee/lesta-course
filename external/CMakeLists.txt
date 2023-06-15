project(external)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL
                 ${CMAKE_BINARY_DIR}/lib/SDL)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad
                 ${CMAKE_BINARY_DIR}/lib/glad)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Catch2
                 ${CMAKE_BINARY_DIR}/lib/Catch2)
