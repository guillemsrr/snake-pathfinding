// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <imgui.h>
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
    static constexpr uint64_t AI_GAME_STEP_INTERVAL_MS = 1;
    static constexpr uint64_t MANUAL_GAME_STEP_INTERVAL_MS = 400;

public:
    GameManager();
    ~GameManager();

    void Init();
    void Iterate(uint64_t currentTime);
    SDL_AppResult HandleEvent(const SDL_Event& event);
    void RenderGame();
    void RenderHUD();
    void Quit();
    ImU32 GetHUDColor();

private:
    uint64_t _currentGameStepIntervalMs;
    uint64_t _intervalLagMs = 0;
    uint64_t _intervalSum = 10;

    unsigned int _seed;

    Camera* _camera;

    bool _isPaused = false;

    const int _size = 10;
    uvec3 _dimensions = {_size, _size, _size};

    Renderer _renderer;

    bool _manualMovement;
    bool _mouseRotating = false;

    IPathfinder* _pathfinder;
    Path _path;

    MapGenerator _mapGenerator;
    GameMap _gameMap;

    Grid _grid;

    int _score = 0;

    uint64_t _lastGameStepTime;

    void Iterate();

    void SetManualMovement(bool isManual);

    void HandleScanCode(SDL_Scancode scancode);
    void HandleSnakeMovementQE(SDL_Scancode scancode) const;
};