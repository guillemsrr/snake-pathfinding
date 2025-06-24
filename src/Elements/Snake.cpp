// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Snake.h"

#include <SDL3/SDL_rect.h>

Snake::Snake()
{
}

void Snake::Move()
{
    auto head = GetHead();
    int32_t newX = head.x;
    int32_t newY = head.y;
    int32_t newZ = head.z;

    /*switch (_direction)
    {
    case SnakeDirection::Right: newX++; break;
    case SnakeDirection::Up:    newY--; break;
    case SnakeDirection::Left:  newX--; break;
    case SnakeDirection::Down:  newY++; break;
    }*/

    /*if (newX < 0) newX = GAME_WIDTH - 1;
    if (newX >= GAME_WIDTH) newX = 0;
    if (newY < 0) newY = GAME_HEIGHT - 1;
    if (newY >= GAME_HEIGHT) newY = 0;
    */

    _body.insert(_body.begin(), {newX, newY, newZ});
    _body.pop_back();
}

void Snake::Grow()
{
    auto tail = _body.back();
    _body.push_back(tail);
}

bool Snake::CheckSelfCollision() const
{
    const auto& head = GetHead();
    for (size_t i = 1; i < _body.size(); ++i)
    {
        if (head == _body[i])
        {
            return true;
        }
    }
    return false;
}

uvec3 Snake::GetHead() const
{
    if (_body.empty())
    {
        return uvec3(-1, -1, -1);
    }

    return _body.front();
}