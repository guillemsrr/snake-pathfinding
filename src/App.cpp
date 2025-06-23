// Copyright (c) Guillem Serra. All Rights Reserved.
#include "App.h"

#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include "Core/GameManager.h"

extern "C" {
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!SDL_SetAppMetadata(APP_TITLE, "0.1", ""))
    {
        SDL_Log("Failed to set app metadata: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AppState* appStateInstance = static_cast<AppState*>(SDL_calloc(1, sizeof(AppState)));
    if (!appStateInstance)
    {
        SDL_Log("Failed to allocate AppState: %s", SDL_GetError());
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    *appstate = appStateInstance;

    if (!SDL_CreateWindowAndRenderer(APP_TITLE,
                                     WINDOW_WIDTH,
                                     WINDOW_HEIGHT,
                                     0,
                                     &appStateInstance->Window,
                                     &appStateInstance->Renderer))
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        SDL_free(appStateInstance);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    GameManager* gameManager = new GameManager();
    if (!gameManager->Init(appStateInstance->Window, appStateInstance->Renderer))
    {
        SDL_Log("Failed to initialize GameManager.");
        delete gameManager;
        SDL_DestroyRenderer(appStateInstance->Renderer);
        SDL_DestroyWindow(appStateInstance->Window);
        SDL_free(appStateInstance);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    appStateInstance->GameInstance = gameManager;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* appStateInstance = static_cast<AppState*>(appstate);
    appStateInstance->GameInstance->Iterate(SDL_GetTicks());

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    AppState* appStateInstance = static_cast<AppState*>(appstate);
    return appStateInstance->GameInstance->HandleEvent(*event);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (appstate != nullptr)
    {
        AppState* appStateInstance = static_cast<AppState*>(appstate);
        if (appStateInstance->GameInstance)
        {
            appStateInstance->GameInstance->Quit();
            delete appStateInstance->GameInstance;
        }

        SDL_DestroyRenderer(appStateInstance->Renderer);
        SDL_DestroyWindow(appStateInstance->Window);
        SDL_free(appStateInstance);
    }

    SDL_Quit();
}
}