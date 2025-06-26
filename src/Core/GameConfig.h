// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

constexpr uint32_t GAME_WIDTH = 20U;
constexpr uint32_t GAME_HEIGHT = 20U;

constexpr int32_t BLOCK_SIZE_PIXELS = 24;
constexpr GLsizei WINDOW_WIDTH = BLOCK_SIZE_PIXELS * GAME_WIDTH;
constexpr GLsizei WINDOW_HEIGHT = BLOCK_SIZE_PIXELS * GAME_HEIGHT;