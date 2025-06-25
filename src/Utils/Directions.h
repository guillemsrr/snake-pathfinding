// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include <glm/vec3.hpp>

class Directions
{
public:
    static constexpr auto None = glm::ivec3(0, 0, 0);

    static constexpr auto Right = glm::ivec3(1, 0, 0);
    static constexpr auto Left = glm::ivec3(-1, 0, 0);
    static constexpr auto Up = glm::ivec3(0, 1, 0);
    static constexpr auto Down = glm::ivec3(0, -1, 0);
    static constexpr auto Forward = glm::ivec3(0, 0, 1);
    static constexpr auto Back = glm::ivec3(0, 0, -1);

    static constexpr glm::ivec3 DirectionsArray[6] = {Right, Left, Up, Down, Forward, Back};

    static glm::ivec3 ToDominantDirection(const glm::vec3& v);
};