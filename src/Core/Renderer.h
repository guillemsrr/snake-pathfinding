// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once
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
    void RenderDirection(const std::vector<Cell*>& cells);

private:
    Camera* _camera;
    unsigned int _generalShader;

    CubeRenderer _cubeRenderer;

    Theme _theme;
};