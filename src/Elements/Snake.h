// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <vector>

#include "Utils/Types.h"

class Snake
{
public:
    Snake();

    void Move();
    void Grow();
    bool CheckSelfCollision() const;
    uvec3 GetHead() const;

    const std::vector<uvec3>& GetBody() const
    {
        return _body;
    }

private:
    std::vector<uvec3> _body;
};