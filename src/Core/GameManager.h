// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "Renderer.h"
#include "Map/GameMap.h"
#include "Map/Grid.h"
#include "Map/MapGenerator.h"

#include "Pathfinding/Base/Path.h"

#include <SDL3/SDL.h>

class IPathfinder;
class Camera;

class GameManager
{
    static constexpr uint64_t AI_GAME_STEP_INTERVAL_MS = 50;
    static constexpr uint64_t MANUAL_GAME_STEP_INTERVAL_MS = 400;

public:
    GameManager();
    ~GameManager();

    void Init();
    void Iterate(uint64_t currentTime);
    SDL_AppResult HandleEvent(const SDL_Event& event);
    void HandleScanCode(SDL_Scancode scancode);
    void Quit();

    void RenderGame();

private:
    uint64_t _currentGameStepIntervalMs;

    unsigned int _seed;

    Camera* _camera;



    const int _size = 5;
    uvec3 _dimensions = {_size, _size, 1};
    
    Renderer _renderer;

    bool _manualMovement;

    IPathfinder* _pathfinder;
    Path _path;

    MapGenerator _mapGenerator;
    GameMap _gameMap;

    Grid _grid;

    uint64_t _lastGameStepTime;

    void SetManualMovement(bool isManual);

    void HandleSnakeMovementQE(SDL_Scancode scancode) const;
};