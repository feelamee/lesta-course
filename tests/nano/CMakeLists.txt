project(tests-engine)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/transform
                 ${CMAKE_CURRENT_BINARY_DIR}/transform)

add_executable(
  read-file-test
  read-file.cpp
)

target_link_libraries(read-file-test PUBLIC nano)
target_link_libraries(read-file-test PUBLIC Catch2::Catch2WithMain)
