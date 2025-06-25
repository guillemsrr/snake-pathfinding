// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include <vector>

class Cell;

class IGraph
{
public:
    virtual ~IGraph() = default;
    virtual std::vector<Cell*> GetNeighbors(Cell* cell) const = 0;
};