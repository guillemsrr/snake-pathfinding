// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>

class GameManager;

constexpr uint64_t STEP_RATE_MILLISECONDS = 125;
inline const char* APP_TITLE = "Snake AI 3D";

struct AppState
{
    SDL_Window* Window;
    SDL_GLContext GlContext;
    GameManager* GameInstance;
};

extern "C" {
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]);
SDL_AppResult SDL_AppIterate(void* appstate);
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event);
void SDL_AppQuit(void* appstate, SDL_AppResult result);
}