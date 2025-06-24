// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <vector>
#include "Utils/Types.h"

class Snake
{
public:
    Snake();

    uvec3 GetNextHeadLocation() const;
    void Move();
    void Grow();
    bool CheckSelfCollision() const;
    void SetDirection(glm::ivec3 direction);

    uvec3 GetHeadLocation() const;

    const std::vector<uvec3>& GetBody() const
    {
        return _body;
    }

private:
    std::vector<uvec3> _body;

    uvec3 _direction;
};