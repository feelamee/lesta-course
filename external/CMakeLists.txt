project(external)

if (ANDROID)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL
                 ${CMAKE_BINARY_DIR}/lib/SDL_${ANDROID_ABI})

set(SDL3IMAGE_VENDORED ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL_image
                 ${CMAKE_BINARY_DIR}/lib/SDL_image_${ANDROID_ABI})
else()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL
                 ${CMAKE_BINARY_DIR}/lib/SDL)

set(SDL3IMAGE_VENDORED ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL_image
                 ${CMAKE_BINARY_DIR}/lib/SDL_image)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad
                 ${CMAKE_BINARY_DIR}/lib/glad)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Catch2
                 ${CMAKE_BINARY_DIR}/lib/Catch2)

