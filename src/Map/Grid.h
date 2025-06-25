// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "Cell.h"
#include <vector>
#include "Utils/Types.h"

class Grid
{
public:
    Grid();
    ~Grid();

    Cell* GetCell(uvec3 position) const;
    void SetDimensions(uvec3 dimensions);

    uvec3 GetDimensions() const
    {
        return _dimensions;
    }

    std::vector<Cell*> GetCells() const
    {
        return _cells;
    }

    std::vector<Cell*> GetNeighbors(Cell* cell) const;
    bool IsLocationValid(uvec3 vec) const;

private:
    std::vector<Cell*> _cells;

    uvec3 _dimensions = {};

    glm::u32 GetIndex(uvec3 position) const;
    uvec3 GetCoords(glm::u32 index) const;
};