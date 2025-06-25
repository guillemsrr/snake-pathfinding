// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "Graphics/CubeRenderer.h"

#include "Map/GameMap.h"
#include "Map/Grid.h"
#include "Map/MapGenerator.h"

#include "Pathfinding/Base/Path.h"

#include <SDL3/SDL.h>

class IPathfinder;
class Camera;

class GameManager
{
    static constexpr uint64_t GAME_STEP_INTERVAL_MS = 50; //400

public:
    GameManager();
    ~GameManager();

    void Init();
    void Iterate(uint64_t currentTime);
    SDL_AppResult HandleEvent(const SDL_Event& event);
    void Quit();

    void RenderGame();

private:
    Camera* _camera;
    float _yaw = 0.f;
    float _pitch = 0.f;
    float _radius = 10.f;

    unsigned int _gridCellShader;

    bool _manualMovement = false;

    IPathfinder* _pathfinder;
    Path _path;

    MapGenerator _mapGenerator;
    GameMap _gameMap;

    Grid _grid;

    uint64_t _lastGameStepTime;

    CubeRenderer _cubeRenderer;

    void HandleSnakeMovement(const SDL_Event& event);
};