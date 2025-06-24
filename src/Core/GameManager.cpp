// Copyright (c) Guillem Serra. All Rights Reserved.

#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include "GameConfig.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Camera.h"
#include "Graphics/GraphicsUtils.h"

GameManager::GameManager(): _lastGameStepTime(0)
{
    srand(time(nullptr));
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
    _target.Respawn();
    _lastGameStepTime = SDL_GetTicks();

    _camera = new Camera(glm::vec3(0, 0, 5), glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    _grid = Grid();
    uvec3 dimensions = {5, 5, 5};
    _grid.SetDimensions(dimensions);
    _mapGenerator = MapGenerator();
    _mapGenerator.Generate(_grid);

    _cubeRenderer.Init();

    _gridCellShader = GraphicsUtils::LoadShader("basic.vert", "basic.frag");
    if (_gridCellShader == 0)
    {
        SDL_Log("Failed to load shader program. Check file paths and console for errors.");
    }
}

void GameManager::Iterate(uint64_t currentTime)
{
    if (currentTime > _lastGameStepTime + GAME_STEP_INTERVAL_MS)
    {
        _lastGameStepTime = currentTime;

        //_snake.Move();
        //CheckCollisions();
    }
}

SDL_AppResult GameManager::HandleEvent(const SDL_Event& event)
{
    if (event.key.scancode == SDL_SCANCODE_ESCAPE)
    {
        return SDL_APP_SUCCESS;
    }

    switch (event.type)
    {
    case SDL_EVENT_QUIT:
    case SDL_SCANCODE_ESCAPE:
        return SDL_APP_SUCCESS;
    case SDL_SCANCODE_R:
        //Restart
        break;
    case SDL_EVENT_MOUSE_MOTION:
        {
            int mouseX = static_cast<int>(event.motion.xrel);
            int mouseY = static_cast<int>(event.motion.yrel);
            _yaw -= mouseX * 0x00080000;
            _pitch = SDL_max(-0x40000000, SDL_min(0x40000000, _pitch - mouseY * 0x00080000));
            break;
        }
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

void GameManager::Quit()
{
}

void GameManager::CheckCollisions()
{
    /*if (_snake.GetHead() == _target.Position)
    {
        _snake.Grow();
        _target.Respawn();
    }

    if (_snake.CheckSelfCollision())
    {
        //TODO
    }*/
}

void GameManager::RenderGame()
{
    float camX = _radius * cosf(_pitch) * sinf(_yaw);
    float camY = _radius * sinf(_pitch);
    float camZ = _radius * cosf(_pitch) * cosf(_yaw);
    glm::vec3 camPos = glm::vec3(camX, camY, camZ);

    _camera->SetPosition(camPos);

    uvec3 dimensions = _grid.GetDimensions();
    glm::vec3 center = glm::vec3(dimensions) * 0.5f;
    _camera->SetTarget(center);

    glUseProgram(_gridCellShader);

    glm::mat4 view = _camera->GetViewMatrix();
    glm::mat4 proj = _camera->GetProjectionMatrix();

    GLint loc_mvp = glGetUniformLocation(_gridCellShader, "uMVP");

    for (Cell* cell : _grid.GetCells())
    {
        float spacing = 1.1f;
        glm::vec3 worldPos = glm::vec3(cell->GetGridPosition()) * spacing;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos);
        model = glm::scale(model, glm::vec3(0.9f));
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, &mvp[0][0]);
        _cubeRenderer.Draw();
    }
}