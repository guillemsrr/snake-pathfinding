// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <vector>
#include "ElementBase.h"

#include "Core/GameConfig.h"

class Snake: public ElementBase
{
public:
    Snake();

    void Move();
    void Grow();
    void SetDirection(SnakeDirection direction);
    bool CheckSelfCollision() const;
    Vector2 GetHead() const;

    const std::vector<Vector2>& GetBody() const {return _body;}
    
private:
    std::vector<Vector2> _body;
    SnakeDirection _direction;
};