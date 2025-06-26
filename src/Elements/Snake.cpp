// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Snake.h"
#include <glm/geometric.hpp>
#include <algorithm>

Snake::Snake(): _direction()
{
    Reset();
}

uvec3 Snake::GetNextHeadLocation() const
{
    uvec3 headLocation = GetHeadLocation();
    headLocation += _direction;
    return headLocation;
}

void Snake::Move()
{
    if (_direction == glm::ivec3())
    {
        return;
    }

    for (size_t i = _body.size() - 1; i > 0; --i)
    {
        _body[i] = _body[i - 1];
    }

    _body[0] = GetNextHeadLocation();
}

void Snake::Grow()
{
    uvec3 tail = _body.back();
    _body.push_back(tail);
}

bool Snake::CheckSelfCollision() const
{
    const auto& head = GetHeadLocation();
    for (size_t i = 1; i < _body.size(); ++i)
    {
        if (head == _body[i])
        {
            return true;
        }
    }
    return false;
}

uvec3 Snake::GetHeadLocation() const
{
    return _body.front();
}

uvec3 Snake::GetTailLocation() const
{
    return _body.back();
}

bool Snake::IsAnyBodyPartInPosition(uvec3 position) const
{
    if (GetTailLocation() == position)
    {
        return false;
    }

    return std::ranges::any_of(_body,
                               [position](const uvec3& segment)
                               {
                                   return segment == position;
                               });
}

void Snake::Reset()
{
    _body.clear();
    _body.emplace_back(0, 0, 0);
    _direction = glm::ivec3();
}

int Snake::GetLength() const
{
    return _body.size();
}

void Snake::SetDirection(glm::ivec3 direction)
{
    if (direction == glm::ivec3())
    {
        _direction = direction;
        return;
    }

    if (glm::length(glm::vec3(direction)) != 1.f)
    {
        return;
    }

    _direction = direction;
}