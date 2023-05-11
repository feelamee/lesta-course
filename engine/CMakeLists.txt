project(nano-engine)

set(TARGET ${PROJECT_NAME})
add_library(
    ${TARGET} SHARED
    src/engine.cpp
    src/engine.hpp
    src/event.cpp
    src/raii_wrapper.hpp
    src/shader.cpp
    src/vertex.cpp
    include/vertex.hpp
    include/color.hpp
    include/iengine.hpp
    include/event.hpp
)
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${TARGET} PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)
target_link_libraries(${TARGET} PUBLIC SDL3-shared)

find_library(FIND_GLAD_RESULT glad)
target_link_libraries(${TARGET} PUBLIC glad)
