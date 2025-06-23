// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <cstdint>

constexpr uint32_t GAME_WIDTH = 24U;
constexpr uint32_t GAME_HEIGHT = 18U;

constexpr int32_t BLOCK_SIZE_PIXELS = 24;
constexpr int32_t WINDOW_WIDTH = BLOCK_SIZE_PIXELS * static_cast<int32_t>(GAME_WIDTH);
constexpr int32_t WINDOW_HEIGHT = BLOCK_SIZE_PIXELS * static_cast<int32_t>(GAME_HEIGHT);

enum class SnakeDirection : uint8_t
{
    Right,
    Up,
    Left,
    Down
};