// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include "Elements/Snake.h"
#include "Elements/Target.h"

#include "Map/Grid.h"
#include "Map/MapGenerator.h"

#include <SDL3/SDL.h>

class GameManager
{
public:
    GameManager();
    ~GameManager();

    bool Init(SDL_Window* window, SDL_Renderer* renderer);
    void Iterate(uint64_t currentTime);
    SDL_AppResult HandleEvent(const SDL_Event& event);
    void Quit();

private:
    void RenderGame();
    void CheckCollisions();

    SDL_Window* _window;
    SDL_Renderer* _renderer;

    Snake _snake;
    MapGenerator _mapGenerator;
    Target _target;
    Grid _grid;

    uint64_t _lastGameStepTime;
};