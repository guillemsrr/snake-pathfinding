# snAI-k 3D (Snake 3D AI pathfinding)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.x-red.svg)](https://www.opengl.org/)
[![SDL](https://img.shields.io/badge/SDL-3.2.16-green.svg)](https://github.com/libsdl-org/SDL)
[![GLM](https://img.shields.io/badge/GLM-1.0.1--3-blue.svg)](https://github.com/g-truc/glm)
[![GLAD](https://img.shields.io/badge/GLAD-0.1.36-lightgrey.svg)](https://github.com/Dav1dde/glad)

## Overview
This project implements a classic Snake game prototype that features AI pathfinding algorithms, SDL3 for window and input, OpenGL for rendering, ImGui for GUI controls, and a dynamic synth sound generation.

🎮 **Test snAI-k on itch.io!**  
[snAI-k on itch.io](https://guillem-serra.itch.io/snai-k)

![Light Snake](images/light_snake1.gif)

## Features

- AI Pathfinding: Implements an AI-controlled snake using A* pathfinding to navigate towards targets.
- Rendering: Real-time rendering using OpenGL with SDL3 window management.
- Audio: Custom audio synthesizer based on snake behavior and environment, supporting audio mixing.
- User Interface: integrated ImGui for a simple HUD.

## Dependencies
- OpenGL 3
- SDL 3
- glm 1
- glad 0.1

![Dark Snake](images/dark_snake1.gif)

## Building

This project uses CMake and relies on vcpkg for dependency management.
You also need to manually clone Dear ImGui into third_party/imgui directory.

Generic build commands:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Building with vcpkg on Windows (Visual Studio 2022)
```bash
cmake -B build/native -S . ^
-DCMAKE_TOOLCHAIN_FILE=F:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ^
-DVCPKG_TARGET_TRIPLET=x64-windows ^
-G "Visual Studio 17 2022" -A x64

cmake --build build/native --config Release
```

# Documentation:

#### SDL3 examples
https://examples.libsdl.org/SDL3/demo/01-snake/  
https://examples.libsdl.org/SDL3/demo/02-woodeneye-008/  
https://examples.libsdl.org/SDL3/audio/02-simple-playback-callback/

#### OpenGL
https://learnopengl.com/

#### A* pathfinding algorithm
https://www.redblobgames.com/pathfinding/a-star/introduction.html

## License

This project is open source and available under the MIT License.