// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <imgui.h>
#include "Renderer.h"

#include "Audio/AudioEngine.h"

#include "core/GameBase.h"

#include "Map/GameMap.h"
#include "Map/Grid.h"
#include "Map/MapGenerator.h"

#include "Pathfinding/Base/Path.h"

#include <SDL3/SDL.h>

class IPathfinder;
class Camera;

class GameManager : public SerraEngine::GameBase
{
    static constexpr float AI_GAME_STEP_INTERVAL_MS = 0.001f;
    static constexpr float MANUAL_GAME_STEP_INTERVAL_MS = 0.05f;

public:
    GameManager();

    virtual void Init(SDL_Window* window);

    void Update(float deltaTime) override;
    void HandleEvent(const SDL_Event& e) override;
    void Render() override;

    void PlayMoveSound(float duration);
    void Quit() override;

private:
    float _lastGameStepTime = 0;
    float _currentGameStepIntervalMs;
    float _intervalLagMs = 0;
    const float _intervalSum = 0.01f;
    const float maxIntervalLag = 2.f;
    float currentTime = 0;

    float minNote = 100.f;
    float maxNote = 600.0f;

    unsigned int _seed;

    Camera* _camera;

    bool _isPaused = false;

    const int _size = 10;
    uvec3 _dimensions = {_size, _size, _size};

    Renderer _renderer;
    AudioEngine _audioEngine;

    bool _manualMovement;
    bool _mouseRotating = false;

    IPathfinder* _pathfinder;
    Path _path;

    MapGenerator _mapGenerator;
    GameMap _gameMap;

    Grid _grid;

    int _score = 0;

    float CalculateFrequency();
    void Iterate(float deltaTime);
    void RenderUI() override;
    ImU32 GetHUDColor() override;

    void SetManualMovement(bool isManual);

    void HandleScanCode(SDL_Scancode scancode);
    void HandleSnakeMovementQE(SDL_Scancode scancode) const;
};