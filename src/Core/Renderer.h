// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
#include <imgui.h>
#include <vector>

#include "Graphics/CubeRenderer.h"
#include "Graphics/Color/Theme.h"

#include "Map/Cell.h"

struct Path;
class GameMap;
class Grid;
class Camera;

class Renderer
{
public:
    Renderer(Camera* camera);

    void RenderBackground();
    void RenderGrid(const Grid& grid);
    void RenderGameMap(const GameMap& gameMap);
    void RenderPath(const Path& path);
    void RenderHorizontalDirection(const std::vector<Cell*>& cells);
    void RenderVerticalDirection(const std::vector<Cell*>& cells);
    void RenderForwardDirection(const std::vector<Cell*>& cells);

    void SwapTheme();
    ImU32 GetHUDColor();

private:
    Camera* _camera;
    unsigned int _generalShader;

    CubeRenderer _cubeRenderer;

    Theme _theme;
    bool _isLight = false;

    void RenderCells(const std::vector<Cell*>& cells, glm::vec4 color);
};