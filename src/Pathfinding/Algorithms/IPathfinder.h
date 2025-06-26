// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

class IGraph;
class Grid;
class Cell;
struct Path;

class IPathfinder
{
public:
    virtual ~IPathfinder() = default;
    virtual Path FindPath(IGraph* graph, Cell* start, Cell* end) = 0;
};