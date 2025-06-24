// Copyright (c) Guillem Serra. All Rights Reserved.

#include "MapGenerator.h"
#include "Cell.h"
#include "Grid.h"

void MapGenerator::Generate(Grid& grid)
{
    for (uint32_t z = 0; z < grid.GetDimensions().z; ++z)
    {
        for (uint32_t y = 0; y < grid.GetDimensions().y; ++y)
        {
            for (uint32_t x = 0; x < grid.GetDimensions().x; ++x)
            {
                uvec3 position = {x, y, z};
                grid.GetCell(position)->SetType(CellType::Empty);
            }
        }
    }
}