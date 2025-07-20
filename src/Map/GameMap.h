// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include <functional>
#include <memory>
#include "Grid.h"

#include "Elements/Snake.h"

#include "Pathfinding/Base/IGraph.h"

class GameMap final : public IGraph
{
public:
    GameMap() = default;
    GameMap(Grid* grid);
    GameMap(const GameMap& other);
    GameMap& operator=(const GameMap& other);

    std::function<void()> onSnakeReachedTarget;

    Snake* GetSnake() const;
    Cell* GetSnakeCell() const;
    Cell* GetTargetCell() const;
    std::vector<Cell*> GetFreeCells() const;

    uvec3 GetTargetLocation() const
    {
        return _targetLocation;
    }

    void ResetTargetLocation();
    void CheckCollisions();

    bool IsCellFree(const Cell* cell) const;
    std::vector<Cell*> GetNeighbors(Cell* cell) const override;
    Cell* GetCell(uvec3 position) const override;

    void Reset();

private:
    Grid* _grid = nullptr;
    std::unique_ptr<Snake> _snake;

    uvec3 _targetLocation = {};
};