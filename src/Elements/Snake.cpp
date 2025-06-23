// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Snake.h"

#include <SDL3/SDL_rect.h>

Snake::Snake()
{
    _direction = SnakeDirection::Right;
}

void Snake::Move()
{
    auto head = _body.front();
    int32_t newX = head.x;
    int32_t newY = head.y;

    switch (_direction)
    {
    case SnakeDirection::Right: newX++; break;
    case SnakeDirection::Up:    newY--; break;
    case SnakeDirection::Left:  newX--; break;
    case SnakeDirection::Down:  newY++; break;
    }

    if (newX < 0) newX = GAME_WIDTH - 1;
    if (newX >= GAME_WIDTH) newX = 0;
    if (newY < 0) newY = GAME_HEIGHT - 1;
    if (newY >= GAME_HEIGHT) newY = 0;

    _body.insert(_body.begin(), {newX, newY});
    _body.pop_back();
}

void Snake::Grow()
{
    auto tail = _body.back();
    _body.push_back(tail);
}

void Snake::SetDirection(SnakeDirection newDirection)
{
    const bool goingRight = _direction == SnakeDirection::Right && newDirection != SnakeDirection::Left;
    const bool goingLeft = _direction == SnakeDirection::Left && newDirection != SnakeDirection::Right;
    const bool goingUp = _direction == SnakeDirection::Up && newDirection != SnakeDirection::Down;
    const bool goingDown = _direction == SnakeDirection::Down && newDirection != SnakeDirection::Up;

    if (goingRight || goingLeft || goingUp || goingDown)
    {
        _direction = newDirection;
    }
}

bool Snake::CheckSelfCollision() const
{
    const auto& head = _body.front();
    for (size_t i = 1; i < _body.size(); ++i)
    {
        if (head == _body[i])
        {
            return true;
        }
    }
    return false;
}

Vector2 Snake::GetHead() const
{
    return _body.front();
}