// Copyright (c) Guillem Serra. All Rights Reserved.
#include "App.h"

#include "Core/GameConfig.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include "Core/GameManager.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

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

    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    AppState* appStateInstance = static_cast<AppState*>(SDL_calloc(1, sizeof(AppState)));
    if (!appStateInstance)
    {
        SDL_Log("Failed to allocate AppState: %s", SDL_GetError());
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    *appstate = appStateInstance;

    SDL_Window* window = SDL_CreateWindow("SnAI-k 3D",
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        SDL_free(appStateInstance);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    appStateInstance->Window = window;

#ifndef __EMSCRIPTEN__
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(2, &displayBounds);
    int centeredX = displayBounds.x + (displayBounds.w - WINDOW_WIDTH) / 2;
    int centeredY = displayBounds.y + (displayBounds.h - WINDOW_HEIGHT) / 2;
    SDL_SetWindowPosition(window, centeredX, centeredY);
#endif

    appStateInstance->GlContext = SDL_GL_CreateContext(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    float main_scale = SDL_GetWindowDisplayScale(window);
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;
    // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, appStateInstance->GlContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifdef __EMSCRIPTEN__
    // Emscripten / WebGL — no glad loader needed
#else
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to initialize GLAD");
        return SDL_APP_FAILURE;
    }
#endif

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    appStateInstance->GameInstance = new GameManager();
    appStateInstance->GameInstance->Init();
    
    return SDL_APP_CONTINUE;
}

bool showDemoWindow = true;

SDL_AppResult SDL_AppIterate(void* appstate)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    AppState* appStateInstance = static_cast<AppState*>(appstate);
    appStateInstance->GameInstance->Iterate(SDL_GetTicks());
    appStateInstance->GameInstance->RenderGame();

    const float DISTANCE_FROM_EDGE = 10.0f;
    ImVec2 window_pos = ImVec2(DISTANCE_FROM_EDGE, DISTANCE_FROM_EDGE);
    ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f); // top-left

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImU32 HUDColor = appStateInstance->GameInstance->GetHUDColor();
    ImGui::PushStyleColor(ImGuiCol_Text, HUDColor);

    ImGui::Begin("HUD",
                 nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing
                 | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

    appStateInstance->GameInstance->RenderHUD();

    ImGui::End();

    window_pos = ImVec2(ImGui::GetIO().DisplaySize.x - DISTANCE_FROM_EDGE, DISTANCE_FROM_EDGE);
    window_pos_pivot = ImVec2(1.0f, 0.0f); // top-right

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.f);

    ImGui::Begin("FPS",
                 nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing
                 | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::PopStyleColor();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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