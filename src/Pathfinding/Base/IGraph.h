// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include <vector>

#include "Utils/Types.h"

class Cell;

class IGraph
{
public:
    virtual ~IGraph() = default;
    
    virtual Cell* GetCell(uvec3 position) const = 0;
    virtual std::vector<Cell*> GetNeighbors(Cell* cell) const = 0;
};