// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "Elements/Snake.h"
#include "Elements/Target.h"

#include "Graphics/CubeRenderer.h"

#include "Map/Grid.h"
#include "Map/MapGenerator.h"

#include <SDL3/SDL.h>

class Camera;

class GameManager
{
    const uint64_t GAME_STEP_INTERVAL_MS = 100;

public:
    GameManager();
    ~GameManager();

    void Init();
    void Iterate(uint64_t currentTime);
    SDL_AppResult HandleEvent(const SDL_Event& event);
    void Quit();

    void RenderGame();

private:
    void CheckCollisions();

    Camera* _camera;
    float _yaw = 0.f;
    float _pitch = 0.f;
    float _radius = 10.f;

    unsigned int _gridCellShader;

    Snake _snake;
    MapGenerator _mapGenerator;
    Target _target;
    Grid _grid;

    uint64_t _lastGameStepTime;

    CubeRenderer _cubeRenderer;
};