project(external)

if (ANDROID)

if (BUILD_SDL3)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL
                 ${CMAKE_BINARY_DIR}/lib/SDL_${ANDROID_ABI})
endif()

if (BUILD_SDL3IMAGE)
set(SDL3IMAGE_VENDORED ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL_image
                 ${CMAKE_BINARY_DIR}/lib/SDL_image_${ANDROID_ABI})
endif()

else()
if (BUILD_SDL3)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL
                 ${CMAKE_BINARY_DIR}/lib/SDL)
endif()

if (BUILD_SDL3IMAGE)
set(SDL3IMAGE_VENDORED ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL_image
                 ${CMAKE_BINARY_DIR}/lib/SDL_image)
endif()

if (BUILD_CATCH2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Catch2
                 ${CMAKE_BINARY_DIR}/lib/Catch2)
endif()
endif()

if (BUILD_GLAD)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad
                 ${CMAKE_BINARY_DIR}/lib/glad)
endif()
