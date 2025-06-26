// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include "IPathfinder.h"

#include "Map/GameMap.h"

class Snake;
class AStarPathfinder;

class SafeSnakePathfinder final : public IPathfinder
{
public:
    SafeSnakePathfinder() = default;
    ~SafeSnakePathfinder() override;

    Path FindPath(IGraph* graph, Cell* start, Cell* end) override;

private:
    AStarPathfinder* _aStarPathdinder;

    bool IsPathTrappable(GameMap* gameMap, const Path& path);
    void SimulateSnakeFollowPath(const GameMap& gameMap, const Path& path);
    int FloodFillAccessibleCellCount(const GameMap& gameMap, Cell* start);
    Path FindLongestSafePath(IGraph* graph, Cell* start, GameMap* gameMapGraph);
};