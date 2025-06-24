// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include "Utils/Types.h"

class CubeRenderer;

enum class CellType
{
    Empty,
    Wall
};

class Cell
{
public:
    Cell(uvec3 position);

    CellType GetType() const;
    void SetType(CellType type);

    uvec3 GetGridPosition() const
    {
        return GridPosition;
    }

private:
    CellType _type;
    uvec3 GridPosition;
};