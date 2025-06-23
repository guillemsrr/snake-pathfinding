// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include <vector>

struct Vector2
{
    int x;
    int y;

    auto operator ==(std::vector<Vector2>::const_reference vector2) const
    {
        return x == vector2.x && y == vector2.y;
    }
};