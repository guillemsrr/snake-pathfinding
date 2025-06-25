// Copyright (c) Guillem Serra. All Rights Reserved.
#include "App.h"

#include "Core/GameConfig.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include "Core/GameManager.h"
#include <glad/glad.h>

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    AppState* appStateInstance = static_cast<AppState*>(SDL_calloc(1, sizeof(AppState)));
    if (!appStateInstance)
    {
        SDL_Log("Failed to allocate AppState: %s", SDL_GetError());
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    *appstate = appStateInstance;

    appStateInstance->Window = SDL_CreateWindow("SnAI-k 3D",
                                                WINDOW_WIDTH,
                                                WINDOW_HEIGHT,
                                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!appStateInstance->Window)
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        SDL_free(appStateInstance);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    /*SDL_Rect displayBounds;
    SDL_GetDisplayBounds(2, &displayBounds);
    int centeredX = displayBounds.x + (displayBounds.w - WINDOW_WIDTH) / 2;
    int centeredY = displayBounds.y + (displayBounds.h - WINDOW_HEIGHT) / 2;
    SDL_SetWindowPosition(appStateInstance->Window, centeredX, centeredY);*/

    appStateInstance->GlContext = SDL_GL_CreateContext(appStateInstance->Window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to initialize GLAD", SDL_GetError());
        SDL_free(appStateInstance);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    appStateInstance->GameInstance = new GameManager();
    appStateInstance->GameInstance->Init();

    SDL_SetWindowRelativeMouseMode(appStateInstance->Window, true);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* appStateInstance = static_cast<AppState*>(appstate);
    appStateInstance->GameInstance->Iterate(SDL_GetTicks());
    appStateInstance->GameInstance->RenderGame();
    SDL_GL_SwapWindow(appStateInstance->Window);

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

        SDL_GL_DestroyContext(appStateInstance->GlContext);
        SDL_DestroyWindow(appStateInstance->Window);
        SDL_free(appStateInstance);
    }

    SDL_Quit();
}
}