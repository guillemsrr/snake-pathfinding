// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <vector>
class Grid;
class Cell;

class IPathfinder
{
public:
    virtual ~IPathfinder() = default;
    virtual std::vector<Cell*> FindPath(Grid& grid, Cell* start, Cell* end) = 0;
};