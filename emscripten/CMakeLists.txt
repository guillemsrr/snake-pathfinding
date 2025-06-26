cmake_minimum_required(VERSION 3.20)
project(Snake-AI-3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Configuring for Web (Emscripten)")

# Output as .html for Emscripten
set(CMAKE_EXECUTABLE_SUFFIX ".html")
set(CMAKE_EXECUTABLE_OUTPUT_NAME "index")

# Memory growth
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")

# Enable WebGL 2
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sUSE_WEBGL2=1")

# Fetch dependencies
include(FetchContent)

# SDL3 (static)
set(SDL3_DISABLE_INSTALL ON)
set(SDL3_DISABLE_SHARED ON)
set(SDL3_DISABLE_TESTS ON)
FetchContent_Declare(
  sdl3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.16
)
FetchContent_MakeAvailable(sdl3)

# GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
)
FetchContent_MakeAvailable(glad)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# Source files
file(GLOB_RECURSE SOURCES
  "${CMAKE_SOURCE_DIR}/../src/*.cpp"
  "${CMAKE_SOURCE_DIR}/../src/*.h"
)

# ImGui (manually added)
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/../third_party/imgui")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

list(APPEND SOURCES ${IMGUI_SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${sdl3_SOURCE_DIR}/include
  ${glad_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
  "${CMAKE_SOURCE_DIR}/../src"
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE SDL3::SDL3
)

# Shader embedding into Emscripten virtual filesystem
set(SHADER_VIRTUAL_DIR "/shaders")
set(SHADER_HOST_DIR "${CMAKE_SOURCE_DIR}/../src/Graphics/Shaders")
file(GLOB SHADER_FILES "${SHADER_HOST_DIR}/*")
file(COPY ${SHADER_FILES} DESTINATION "${CMAKE_BINARY_DIR}/shaders")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_BINARY_DIR}/shaders@${SHADER_VIRTUAL_DIR}")

target_compile_definitions(${PROJECT_NAME} PRIVATE SHADER_DIR="${SHADER_VIRTUAL_DIR}")
