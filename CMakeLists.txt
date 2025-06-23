cmake_minimum_required(VERSION 3.20)
project(snake-pathfinding)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH
    "F:/Other/SDL_Projects/SDL/cmake"
    "F:/Other/SDL_Projects/SDL_Image/cmake"
)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL3::SDL3 SDL3_image::SDL3_image
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/Other/SDL_Projects/SDL/lib/x64/SDL3.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/Other/SDL_Projects/SDL_Image/lib/x64/SDL3_image.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()