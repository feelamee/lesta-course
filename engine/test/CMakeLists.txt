project(test)

add_executable(
  ${PROJECT_NAME}
  main.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC "-g3")

find_library(FIND_RESULT nano-engine)
target_link_libraries(${PROJECT_NAME} PRIVATE nano-engine)

option(ADDRESS_SANITIZER
       [[Use address sanitizer to find out memory bug.
Works only on Linux. You should already install library:
```sudo dnf install libasan```
To see line numbers in asan log configure your build with
```CMAKE_BUILD_TYPE=Debug```.]] ON)

if(ADDRESS_SANITIZER)
    message(
        STATUS
            "Using address sanitizer")
    add_library(sanitizer INTERFACE)
    target_compile_options(sanitizer INTERFACE -fsanitize=address)
    target_link_options(sanitizer INTERFACE -fsanitize=address)

    target_link_libraries(${PROJECT_NAME} PRIVATE sanitizer)
endif()
