// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "IPathfinder.h"
#include "Utils/Types.h"

class Node;

class AStarPathfinder final : public IPathfinder
{
public:
    Path FindPath(IGraph& graph, Cell* start, Cell* end) override;

private:
    float CalculateHeuristic(vec3 a, vec3 b) const;
    Path GetPath(Node* start, Node* current) const;
};