// Copyright (c) Guillem Serra. All Rights Reserved.

#include "MapGenerator.h"
#include "Cell.h"
#include "Grid.h"

void MapGenerator::Generate(Grid& grid)
{
    for (uint32_t y = 0; y < grid.GetHeight(); ++y)
    {
        for (uint32_t x = 0; x < grid.GetWidth(); ++x)
        {
            if (y == 0 || y == grid.GetHeight() - 1 || x == 0 || x == grid.GetWidth() - 1)
            {
                grid.GetCell(x, y).SetType(CellType::Wall);
            }
            else
            {
                grid.GetCell(x, y).SetType(CellType::Empty);
            }
        }
    }
}