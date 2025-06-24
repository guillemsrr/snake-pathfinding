// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Grid.h"

Cell* Grid::GetCell(uvec3 position)
{
    if (position.x < 0 || position.x >= _dimensions.x || position.y < 0 || position.y >= _dimensions.y || position.z < 0
        || position.z >= _dimensions.z)
    {
        return nullptr;
    }

    int index = Index(position);
    if (index < 0 || index >= _cells.size())
    {
        return nullptr;
    }
    return _cells[index];
}

void Grid::SetDimensions(uvec3 dimensions)
{
    if (dimensions.x <= 0)
    {
        dimensions.x = 1;
    }
    if (dimensions.y <= 0)
    {
        dimensions.y = 1;
    }
    if (dimensions.z <= 0)
    {
        dimensions.z = 1;
    }

    _dimensions = dimensions;

    const int totalSize = dimensions.x * dimensions.y * dimensions.z;
    //uvec3 offset = {static_cast<float>(dimensions.x) / 2.f, 0.f, static_cast<float>(depth) / 2.f};

    if (_cells.size() != totalSize)
    {
        _cells.resize(totalSize);
    }

    for (int z = 0; z < dimensions.z; ++z)
    {
        for (int y = 0; y < dimensions.y; ++y)
        {
            for (int x = 0; x < dimensions.x; ++x)
            {
                uvec3 position = {x, y, z};
                int idx = Index(position);
                _cells[idx] = new Cell(position);
                /*uvec3 position = {
                    static_cast<float>(x) - offset.x,
                    static_cast<float>(y) - offset.y,
                    static_cast<float>(z) - offset.z
                };
                _cells[idx].Position = position;*/
            }
        }
    }

    for (glm::u32 z = 0; z < dimensions.z; ++z)
    {
        for (int y = 0; y < dimensions.y; ++y)
        {
            for (int x = 0; x < dimensions.x; ++x)
            {
                uvec3 position = {x, y, z};
                Cell* currentCell = GetCell(position);
                if (!currentCell)
                {
                    continue;
                }
            }
        }
    }
}

glm::u32 Grid::Index(uvec3 position) const
{
    return position.x + position.y * _dimensions.x + position.z * _dimensions.x * _dimensions.y;
}