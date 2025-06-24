// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <cstdint>

#include <glad/glad.h>

constexpr uint32_t GAME_WIDTH = 40U;
constexpr uint32_t GAME_HEIGHT = 40U;

constexpr int32_t BLOCK_SIZE_PIXELS = 24;
constexpr GLsizei WINDOW_WIDTH = BLOCK_SIZE_PIXELS * GAME_WIDTH;
constexpr GLsizei WINDOW_HEIGHT = BLOCK_SIZE_PIXELS * GAME_HEIGHT;