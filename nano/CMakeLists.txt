project(nano)

set(TARGET ${PROJECT_NAME})
add_library(
    ${TARGET} SHARED

    src/engine.cpp
    src/event.cpp
    src/shader.cpp
    src/transform.cpp
    src/vertbuf.cpp
    src/texture.cpp
    src/shader.cpp
    src/shape.cpp
    src/canvas.cpp
    src/error.cpp
    src/utils.cpp
    src/soundbuf.cpp
    src/resource_loader.cpp
    src/sprite.cpp
    src/sound.cpp

    include/nano/sound.hpp
    include/nano/sprite.hpp
    include/nano/soundbuf.hpp
    include/nano/raii_wrapper.hpp
    include/nano/utils.hpp
    include/nano/error.hpp
    include/nano/vertbuf.hpp
    include/nano/shape.hpp
    include/nano/shader.hpp
    include/nano/canvas.hpp
    include/nano/resource_loader.hpp
    include/nano/texture.hpp
    include/nano/shape.hpp
    include/nano/vertbuf.hpp
    include/nano/rect.hpp
    include/nano/vec.hpp
    include/nano/vertex.hpp
    include/nano/color.hpp
    include/nano/engine.hpp
    include/nano/event.hpp
    include/nano/transform.hpp
)
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${TARGET} PUBLIC cxx_std_20)

option(NANO_DEBUG "Debug macro for libnano-engine" ON)
if (NANO_DEBUG)
target_compile_options(${TARGET} PUBLIC -DNANO_DEBUG)
target_compile_options(${TARGET} PUBLIC -g)
endif()

target_link_libraries(${TARGET} PUBLIC SDL3::SDL3-shared)
target_link_libraries(${TARGET} PUBLIC ImGui)
target_link_libraries(${TARGET} PUBLIC glad)
