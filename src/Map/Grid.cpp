// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Grid.h"

Grid::Grid(uint32_t width, uint32_t height)
    : _width(width), _height(height)
{
    _cells.resize(_height, std::vector<Cell>(_width));
}

Cell& Grid::GetCell(uint32_t x, uint32_t y)
{
    return _cells[y][x];
}

const Cell& Grid::GetCell(uint32_t x, uint32_t y) const
{
    return _cells[y][x];
}

uint32_t Grid::GetWidth() const
{
    return _width;
}

uint32_t Grid::GetHeight() const
{
    return _height;
}