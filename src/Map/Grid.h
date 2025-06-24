// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "Cell.h"
#include <vector>
#include "Utils/Types.h"

class Grid
{
public:
    Cell* GetCell(uvec3 position);
    void SetDimensions(uvec3 dimensions);

    uvec3 GetDimensions() const
    {
        return _dimensions;
    }

    std::vector<Cell*> GetCells()
    {
        return _cells;
    }

private:
    std::vector<Cell*> _cells;

    uvec3 _dimensions = {};

    glm::u32 Index(uvec3 position) const;
};