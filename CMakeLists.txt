cmake_minimum_required(VERSION 3.20)
project(Snake-AI-3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add SerraEngine as a subdirectory
add_subdirectory(../SerraEngine SerraEngineBuild)

# Find packages
find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)

# ImGui sources (manually specified like in the Makefile)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${GAME_SOURCES}
    ${IMGUI_SOURCES}
)

# Include paths
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link libraries (vcpkg targets)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
        OpenGL::GL
        glad::glad
        glm::glm
	SerraEngine
)

# Include engine headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ../SerraEngine/include
)

# Compile-time definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/src/Graphics/Shaders"
)

install(DIRECTORY src/Graphics/Shaders DESTINATION .)