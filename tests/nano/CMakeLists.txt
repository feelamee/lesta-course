project(tests-engine)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/transform
                 ${CMAKE_CURRENT_BINARY_DIR}/transform)

add_executable(
  read-file-test
  read-file.cpp
)
find_library(FIND_CATCH2_RESULT Catch2WithMain)
find_library(FIND_NANO_ENGINE_RESULT nano)
target_link_libraries(read-file-test PRIVATE Catch2WithMain)
target_link_libraries(read-file-test PUBLIC nano)
