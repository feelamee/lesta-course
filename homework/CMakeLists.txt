project(homework)

option(BUILD_01_HELLO_WORLD "Enable building 01-hello-world" OFF)
option(BUILD_02_SDL_LINKING "Enable building 02-sdl-linking" OFF)
option(BUILD_03_SDL_HOT_NANO_ENGINE "Enable building 03-sdl-hot-nano-engine" OFF)
option(BUILD_04_RENDER_BASICS "Enable building 04-render-basics" OFF)
option(BUILD_05_OPENGL_FIRST_STEP "Enable building 05-OpenGL-first-step" OFF)
option(BUILD_06_AND_GOD_CREATE_SOUNDS "Enable building 06-and-god-create-sounds" OFF)

option(BUILD_01 "Enable building 01-hello-world" OFF)
option(BUILD_02 "Enable building 02-sdl-linking" OFF)
option(BUILD_03 "Enable building 03-sdl-hot-nano-engine" OFF)
option(BUILD_04 "Enable building 04-render-basics" OFF)
option(BUILD_05 "Enable building 05-OpenGL-first-step" OFF)
option(BUILD_06 "Enable building 06-and-god-create-sounds" OFF)

option(BUILD_ALL_HW "Enable building all homeworks" OFF)

if(BUILD_01_HELLO_WORLD OR BUILD_01 OR BUILD_ALL OR BUILD_ALL_HW)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/01-hello-world)
endif()

if(BUILD_02_SDL_LINKING OR BUILD_02 OR BUILD_ALL OR BUILD_ALL_HW)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/02-sdl-linking)
endif()

if(BUILD_03_SDL_HOT_NANO_ENGINE OR BUILD_03 OR BUILD_ALL OR BUILD_ALL_HW)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/03-sdl-hot-nano-engine)
endif()

if(BUILD_04_RENDER_BASICS OR BUILD_04 OR BUILD_ALL OR BUILD_ALL_HW)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/04-render-basics)
endif()

if(BUILD_05_OPENGL_FIRST_STEP OR BUILD_05 OR BUILD_ALL OR BUILD_ALL_HW)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/05-OpenGL-first-step)
endif()

if(BUILD_06_AND_GOD_CREATE_SOUNDS OR BUILD_06 OR BUILD_ALL OR BUILD_ALL_HW)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/06-and-god-create-sounds)
endif()
