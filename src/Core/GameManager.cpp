// Copyright (c) Guillem Serra. All Rights Reserved.

#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include "GameConfig.h"

#include "Elements/Snake.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Camera.h"
#include "Graphics/GraphicsUtils.h"

#include "Pathfinding/Algorithms/AStarPathfinder.h"

#include "Utils/Directions.h"

GameManager::GameManager(): _renderer(nullptr), _lastGameStepTime(0)
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
    int size = 10;
    uvec3 dimensions = {size, size, size};
    _grid.SetDimensions(dimensions);
    _mapGenerator = MapGenerator();
    _mapGenerator.Generate(_grid);

    _gameMap = GameMap(_grid);

    _renderer = Renderer(_camera);

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
        //TODO: Restart
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
    case SDL_EVENT_MOUSE_WHEEL:
        {
            float zoomSensitivity = 0.5f;
            _radius -= event.wheel.y * zoomSensitivity;
            _radius = glm::clamp(_radius, 5.0f, 100.0f);
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
    auto camPos = glm::vec3(camX, camY, camZ);

    uvec3 dimensions = _grid.GetDimensions();
    glm::vec3 center = glm::vec3(dimensions) * 0.5f;
    _camera->SetPosition(center + camPos);
    _camera->SetTarget(center);

    _renderer.RenderBackground();
    _renderer.RenderGrid(_grid);
    _renderer.RenderGameMap(_gameMap);

    if (!_manualMovement && _path.IsValid())
    {
        _renderer.RenderPath(_path);
    }
}