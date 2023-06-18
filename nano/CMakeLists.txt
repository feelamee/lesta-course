project(nano)

set(TARGET ${PROJECT_NAME})
set(imgui ${CMAKE_SOURCE_DIR}/external/imgui)
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

    # creating separate static lib from Dear ImGui occurs multiply definition on windows
    # in addition this method is recommended by Dear ImGui Developers
    ${imgui}/imconfig.h
    ${imgui}/imgui.cpp
    ${imgui}/imgui.h
    ${imgui}/imgui_demo.cpp
    ${imgui}/imgui_draw.cpp
    ${imgui}/imgui_tables.cpp
    ${imgui}/imgui_widgets.cpp
    ${imgui}/imgui_internal.h
    ${imgui}/imstb_rectpack.h
    ${imgui}/imstb_textedit.h
    ${imgui}/imstb_truetype.h

    ${imgui}/backends/imgui_impl_opengl3.h
    ${imgui}/backends/imgui_impl_opengl3.cpp

    ${imgui}/backends/imgui_impl_sdl3.h
    ${imgui}/backends/imgui_impl_sdl3.cpp
)
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${TARGET} PUBLIC cxx_std_20)

option(NANO_DEBUG "Debug macro for libnano" ON)
if (NANO_DEBUG)
target_compile_options(${TARGET} PUBLIC -DNANO_DEBUG)
target_compile_options(${TARGET} PUBLIC -g)
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET} PUBLIC OpenGL::GL)
target_link_libraries(${TARGET} PUBLIC glad)
target_link_libraries(${TARGET} PUBLIC SDL3::SDL3-shared)
target_link_libraries(${TARGET} PUBLIC SDL3_image::SDL3_image)

target_link_libraries(${TARGET} PUBLIC sanitizer)

target_include_directories(${TARGET} PUBLIC ${imgui})
target_include_directories(${TARGET} PUBLIC ${imgui}/backends)
target_compile_definitions(${TARGET} PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
