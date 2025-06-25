// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include "Grid.h"

#include "Pathfinding/Base/IGraph.h"

class Snake;

class GameMap final : public IGraph
{
public:
    GameMap() = default;
    GameMap(Grid& grid);

    Snake* GetSnake() const;
    Cell* GetSnakeCell() const;
    Cell* GetTargetCell() const;

    uvec3 GetTargetLocation() const
    {
        return _targetLocation;
    }

    std::vector<Cell*> GetNeighbors(Cell* cell) const override;

    void ResetTargetLocation();
    void CheckCollisions();

    void Reset();

private:
    Grid* _grid = nullptr;
    Snake* _snake;

    uvec3 _targetLocation = {};
};