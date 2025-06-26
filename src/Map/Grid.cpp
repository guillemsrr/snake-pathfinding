// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Grid.h"

#include "Utils/Directions.h"

Grid::Grid()
{
}

Grid::~Grid()
{
    for (Cell* cell : _cells)
    {
        delete cell;
    }
}

Cell* Grid::GetCell(uvec3 position) const
{
    if (position.x < 0 || position.x >= _dimensions.x || position.y < 0 || position.y >= _dimensions.y || position.z < 0
        || position.z >= _dimensions.z)
    {
        return nullptr;
    }

    int index = GetIndex(position);
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
                int idx = GetIndex(position);
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

glm::u32 Grid::GetIndex(uvec3 position) const
{
    return position.x + position.y * _dimensions.x + position.z * _dimensions.x * _dimensions.y;
}

uvec3 Grid::GetCoords(glm::u32 index) const
{
    uvec3 coords;
    coords.z = index / (_dimensions.x * _dimensions.y);
    index -= coords.z * (_dimensions.x * _dimensions.y);
    coords.y = index / _dimensions.x;
    coords.x = index % _dimensions.x;
    return coords;
}

std::vector<Cell*> Grid::GetNeighbors(Cell* cell) const
{
    std::vector<Cell*> neighbors;
    neighbors.reserve(6);

    const uvec3 basePosition = cell->GetGridPosition();

    for (const glm::ivec3& offset : Directions::DirectionsArray)
    {
        glm::ivec3 neighborPos = glm::ivec3(basePosition.x + offset.x,
                                            basePosition.y + offset.y,
                                            basePosition.z + offset.z);

        if (!IsLocationValid(neighborPos))
        {
            continue;
        }

        uvec3 pos = uvec3(neighborPos.x, neighborPos.y, neighborPos.z);
        if (Cell* neighbor = GetCell(pos))
        {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

bool Grid::IsLocationValid(uvec3 vec) const
{
    return GetCell(vec) != nullptr;
}

std::vector<Cell*> Grid::GetCellsLine(Cell* originCell, const glm::ivec3 direction) const
{
    std::vector<Cell*> result;
    if (!originCell || direction == glm::ivec3(0))
    {
        return result;
    }

    uvec3 currentPos = originCell->GetGridPosition();
    while (true)
    {
        currentPos += direction;
        if (!IsLocationValid(currentPos))
        {
            break;
        }

        Cell* nextCell = GetCell(currentPos);
        if (!nextCell)
        {
            break;
        }
        result.push_back(nextCell);
    }
    return result;
}

std::vector<Cell*> Grid::GetCellsHorizontal(Cell* originCell) const
{
    if (!originCell)
    {
        return {};
    }

    std::vector<Cell*> cells;

    auto frontCells = GetCellsLine(originCell, Directions::Forward);
    auto leftCells = GetCellsLine(originCell, Directions::Left);
    auto rightCells = GetCellsLine(originCell, Directions::Right);

    cells.insert(cells.end(), frontCells.begin(), frontCells.end());
    cells.insert(cells.end(), leftCells.begin(), leftCells.end());
    cells.insert(cells.end(), rightCells.begin(), rightCells.end());

    return cells;
}

std::vector<Cell*> Grid::GetCellsVertical(Cell* originCell) const
{
    if (!originCell)
    {
        return {};
    }

    std::vector<Cell*> cells;
    auto upCells = GetCellsLine(originCell, Directions::Up);
    auto downCells = GetCellsLine(originCell, Directions::Down);

    cells.insert(cells.end(), upCells.begin(), upCells.end());
    cells.insert(cells.end(), downCells.begin(), downCells.end());

    return cells;
}