// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include "Theme.h"

namespace Themes
{
    static constexpr Theme Dark = {
        .BackgroundColor = glm::vec3(0.f),
        .SnakeHeadColor = glm::vec3(1.f),
        .SnakeBodyColor = glm::vec3(0.85f),
        .TargetColor = glm::vec3(1.f, 0.5f, 0.1f),
        .GridLineColor = glm::vec3(0.3f, 0.3f, 0.3f),
        .PathLineColor = glm::vec3(0.9f, 0.4f, 0.f),
        .CellColor = glm::vec3(0.65f),
        .DirectionColor = glm::vec3(0.f, 0.f, 1.f)
    };

    static constexpr Theme Light = {
        .BackgroundColor = glm::vec3(1.f),
        .SnakeHeadColor = glm::vec3(0.f),
        .SnakeBodyColor = glm::vec3(0.15f),
        .TargetColor = glm::vec3(0.6f, 0.1f, 0.0f),
        .GridLineColor = glm::vec3(0.8f, 0.8f, 0.8f),
        .PathLineColor = glm::vec3(0.7f, 0.2f, 0.f),
        .CellColor = glm::vec3(0.35f),
        .DirectionColor = glm::vec3(0.f, 0.f, 1.f)
    };

    static constexpr Theme ColoredDark = {
        .BackgroundColor = glm::vec3(0.1f, 0.1f, 0.15f),
        .SnakeHeadColor = glm::vec3(0.9f, 0.1f, 0.1f),
        .SnakeBodyColor = glm::vec3(0.7f, 0.2f, 0.2f),
        .TargetColor = glm::vec3(1.f, 0.8f, 0.f),
        .GridLineColor = glm::vec3(0.2f, 0.2f, 0.2f),
        .PathLineColor = glm::vec3(0.3f, 0.6f, 1.f),
        .CellColor = glm::vec3(0.3f, 0.3f, 0.3f),
    };

    static constexpr Theme ColoredLight = {
        .BackgroundColor = glm::vec3(1.f, 1.f, 1.f),
        .SnakeHeadColor = glm::vec3(0.f, 0.3f, 0.f),
        .SnakeBodyColor = glm::vec3(0.f, 0.5f, 0.f),
        .TargetColor = glm::vec3(0.9f, 0.2f, 0.f),
        .GridLineColor = glm::vec3(0.9f, 0.9f, 0.9f),
        .PathLineColor = glm::vec3(0.1f, 0.3f, 0.8f),
        .CellColor = glm::vec3(0.95f, 0.95f, 0.95f),
    };
}