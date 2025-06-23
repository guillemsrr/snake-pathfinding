// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "Cell.h"
#include <vector>
#include <cstdint>

class Grid
{
public:
    Grid(uint32_t width, uint32_t height);

    Cell& GetCell(uint32_t x, uint32_t y);
    const Cell& GetCell(uint32_t x, uint32_t y) const;

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    uint32_t _width;
    uint32_t _height;
    std::vector<std::vector<Cell>> _cells;
};