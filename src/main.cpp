// Copyright (c) Guillem SerraEngine. All Rights Reserved.

#include "core/App.h"
#include "Core/GameManager.h"

using namespace SerraEngine;

int main(int argc, char* argv[])
{
    App EngineApp = App(argc, argv);
    if (!EngineApp.Init("snAI-K", 800, 800))
    {
        return 0;
    }

    GameManager game;
    EngineApp.StartGame(game);

    return 0;
}