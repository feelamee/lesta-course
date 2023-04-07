project(homework)

option(BUILD_01_HELLO_WORLD "Enable building 01-hello-world" OFF)
option(BUILD_02_SDL_LINKING "Enable building 02-hello-world" OFF)
option(BUILD_ALL "Enable building all homeworks" OFF)

if(BUILD_01_HELLO_WORLD OR BUILD_ALL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/01-hello-world)
endif()

if(BUILD_02_SDL_LINKING OR BUILD_ALL)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/02-sdl-linking)
endif()
