project(homework)

option(BUILD_01_HELLO_WORLD "Enable building 01-hello-world" OFF)
option(BUILD_02_SDL_LINKING "Enable building 02-sdl-linking" OFF)
option(BUILD_03_SDL_HOT_NANO_ENGINE "Enable building 03-sdl-hot-nano-engine" OFF)
option(BUILD_04_RENDER_BASICS "Enable building 04-render-basics" OFF)
option(BUILD_ALL "Enable building all homeworks" OFF)

if(BUILD_01_HELLO_WORLD OR BUILD_ALL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/01-hello-world)
endif()

if(BUILD_02_SDL_LINKING OR BUILD_ALL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/02-sdl-linking)
endif()

if(BUILD_03_SDL_HOT_NANO_ENGINE OR BUILD_ALL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/03-sdl-hot-nano-engine)
endif()

if(BUILD_04_RENDER_BASICS OR BUILD_ALL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/04-render-basics)
endif()