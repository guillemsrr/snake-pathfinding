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
    uvec3 GetTailLocation() const;

    const std::vector<uvec3>& GetBody() const
    {
        return _body;
    }

    bool IsAnyBodyPartInPosition(uvec3 position) const;
    void Reset();

    glm::ivec3 GetDirection() const
    {
        return _direction;
    }

private:
    std::vector<uvec3> _body;

    glm::ivec3 _direction;
};