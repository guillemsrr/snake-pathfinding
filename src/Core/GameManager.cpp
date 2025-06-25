// Copyright (c) Guillem Serra. All Rights Reserved.

#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include "GameConfig.h"

#include "Elements/Snake.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Camera.h"
#include "Graphics/GraphicsUtils.h"

#include "Pathfinding/Algorithms/AStarPathfinder.h"

#include "Utils/Directions.h"

GameManager::GameManager(): _lastGameStepTime(0)
{
    srand(time(nullptr));
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
    _lastGameStepTime = SDL_GetTicks();

    _camera = new Camera(glm::vec3(0, 0, 5), glm::radians(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

    _grid = Grid();
    uvec3 dimensions = {5, 5, 5};
    _grid.SetDimensions(dimensions);
    _mapGenerator = MapGenerator();
    _mapGenerator.Generate(_grid);

    _gameMap = GameMap(_grid);

    _cubeRenderer.Init();

    _gridCellShader = GraphicsUtils::LoadShader("basic.vert", "basic.frag");
    if (_gridCellShader == 0)
    {
        SDL_Log("Failed to load shader program. Check file paths and console for errors.");
    }

    _gameMap.ResetTargetLocation();
    _pathfinder = new AStarPathfinder();
    _path = _pathfinder->FindPath(_gameMap, _gameMap.GetSnakeCell(), _gameMap.GetTargetCell());
}

void GameManager::Iterate(uint64_t currentTime)
{
    if (currentTime > _lastGameStepTime + GAME_STEP_INTERVAL_MS)
    {
        _lastGameStepTime = currentTime;

        uvec3 nextLocation = _gameMap.GetSnake()->GetNextHeadLocation();
        if (_grid.IsLocationValid(nextLocation))
        {
            _gameMap.GetSnake()->Move();
        }
        else
        {
            SDL_Log("Snake has collided with a wall!");
        }

        _gameMap.CheckCollisions();

        if (!_manualMovement)
        {
            Cell* headCell = _gameMap.GetSnakeCell();
            Cell* targetCell = _gameMap.GetTargetCell();
            _path = _pathfinder->FindPath(_gameMap, headCell, targetCell);

            if (_path.IsValid())
            {
                uvec3 direction = _path.GetSecond()->GetGridPosition() - headCell->GetGridPosition();
                _gameMap.GetSnake()->SetDirection(direction);
            }
            else
            {
                _gameMap.GetSnake()->SetDirection(Directions::None);
            }
        }
    }
}

void GameManager::HandleSnakeMovement(const SDL_Event& event)
{
    switch (event.key.scancode)
    {
    case SDL_SCANCODE_RIGHT:
        _gameMap.GetSnake()->SetDirection(Directions::Right);
        break;
    case SDL_SCANCODE_UP:
        _gameMap.GetSnake()->SetDirection(Directions::Up);
        break;
    case SDL_SCANCODE_LEFT:
        _gameMap.GetSnake()->SetDirection(Directions::Left);
        break;
    case SDL_SCANCODE_DOWN:
        _gameMap.GetSnake()->SetDirection(Directions::Down);
        break;
    default: break;
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
            float sensitivity = 0.005f;

            _yaw -= event.motion.xrel * sensitivity;
            _pitch -= event.motion.yrel * sensitivity;

            const float pitchLimit = glm::radians(89.0f);
            _pitch = glm::clamp(_pitch, -pitchLimit, pitchLimit);
            break;
        }
    default:
        break;
    }

    if (_manualMovement)
    {
        HandleSnakeMovement(event);
    }

    return SDL_APP_CONTINUE;
}

void GameManager::Quit()
{
}

void GameManager::RenderGame()
{
    float camX = _radius * cosf(_pitch) * sinf(_yaw);
    float camY = _radius * sinf(_pitch);
    float camZ = _radius * cosf(_pitch) * cosf(_yaw);
    glm::vec3 camPos = glm::vec3(camX, camY, camZ);

    uvec3 dimensions = _grid.GetDimensions();
    glm::vec3 center = glm::vec3(dimensions) * 0.5f;
    _camera->SetPosition(center + camPos);
    _camera->SetTarget(center);

    glUseProgram(_gridCellShader);

    glm::mat4 view = _camera->GetViewMatrix();
    glm::mat4 proj = _camera->GetProjectionMatrix();

    GLint locMVP = glGetUniformLocation(_gridCellShader, "uMVP");
    GLint locColor = glGetUniformLocation(_gridCellShader, "uColor");
    glUniform4f(locColor, 1.0f, 0.5f, 0.2f, 1.0f);

    for (Cell* cell : _grid.GetCells())
    {
        float spacing = 1.f;
        glm::vec3 worldPos = glm::vec3(cell->GetGridPosition()) * spacing;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos);
        model = glm::scale(model, glm::vec3(1.f));
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvp[0][0]);
        _cubeRenderer.Draw();
    }

    // Render snake body
    for (const auto& segment : _gameMap.GetSnake()->GetBody())
    {
        glm::vec3 worldPos = glm::vec3(segment) * 1.0f;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos);
        model = glm::scale(model, glm::vec3(0.85f));
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvp[0][0]);
        glUniform4f(locColor, 0.0f, 1.0f, 0.0f, 1.0f);
        _cubeRenderer.Draw();
    }

    // Render snake head
    glm::vec3 headPos = glm::vec3(_gameMap.GetSnake()->GetHeadLocation()) * 1.0f;
    glm::mat4 headModel = glm::translate(glm::mat4(1.0f), headPos);
    headModel = glm::scale(headModel, glm::vec3(0.9f));
    glm::mat4 headMvp = proj * view * headModel;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &headMvp[0][0]);
    glUniform4f(locColor, 1.0f, 0.0f, 0.0f, 1.0f);
    _cubeRenderer.Draw();

    // Render target location
    glm::vec3 targetPos = glm::vec3(_gameMap.GetTargetLocation()) * 1.0f;
    glm::mat4 targetModel = glm::translate(glm::mat4(1.0f), targetPos);
    targetModel = glm::scale(targetModel, glm::vec3(0.8f));
    glm::mat4 targetMvp = proj * view * targetModel;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &targetMvp[0][0]);
    glUniform4f(locColor, 1.0f, 0.0f, 1.0f, 1.0f);
    _cubeRenderer.Draw();

    if (!_manualMovement && _path.IsValid())
    {
        for (int i = 0; i < _path.Cells.size() - 2; ++i)
        {
            glm::vec3 start = glm::vec3(_path.Cells[i]->GetGridPosition());
            glm::vec3 end = glm::vec3(_path.Cells[i + 1]->GetGridPosition());

            glm::vec3 direction = end - start;
            float length = glm::length(direction);
            glm::vec3 center = (start + end) * 0.5f;

            glm::vec3 up = glm::vec3(0, 1, 0); // default cube orientation
            glm::mat4 rotation = glm::mat4(1.0f);
            if (glm::length(glm::cross(up, direction)) > 0.001f)
            {
                glm::vec3 axis = glm::normalize(glm::cross(up, direction));
                float angle = acos(glm::dot(glm::normalize(up), glm::normalize(direction)));
                rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
            }

            glm::vec3 scale = glm::vec3(0.1f, length, 0.1f);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
            model = model * rotation;
            model = glm::scale(model, scale);
            glm::mat4 mvp = proj * view * model;
            glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvp[0][0]);
            glUniform4f(locColor, 1.0f, 0.0f, 0.0f, 1.f);
            _cubeRenderer.Draw();
        }
    }
}