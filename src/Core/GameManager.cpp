// Copyright (c) Guillem Serra. All Rights Reserved.

#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include <imgui.h>
#include "GameConfig.h"
#include "Elements/Snake.h"

#include "Graphics/Camera.h"
#include "Graphics/GraphicsUtils.h"

#include "Pathfinding/Algorithms/SafeSnakePathfinder.h"

#include "Utils/Directions.h"

GameManager::GameManager(): _renderer(nullptr), _lastGameStepTime(0)
{
    _seed = time(nullptr);
    srand(_seed);
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
    _lastGameStepTime = SDL_GetTicks();

    _camera = new Camera(glm::vec3(0, 0, 5), glm::radians(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

    _grid = Grid();

    _grid.SetDimensions(_dimensions);

    _mapGenerator = MapGenerator();
    _mapGenerator.Generate(_grid);

    _gameMap = GameMap(&_grid);
    _renderer = Renderer(_camera);

    _gameMap.ResetTargetLocation();
    _pathfinder = new SafeSnakePathfinder();
    _path = _pathfinder->FindPath(&_gameMap, _gameMap.GetSnakeCell(), _gameMap.GetTargetCell());

    SetManualMovement(false);

    _gameMap.onSnakeReachedTarget = [this]()
    {
        _score += 1;
    };
}

void GameManager::Iterate(uint64_t currentTime)
{
    if (_isPaused)
    {
        return;
    }

    uint64_t targetTime = _lastGameStepTime + _currentGameStepIntervalMs + _intervalLagMs;
    if (currentTime > targetTime)
    {
        _lastGameStepTime = currentTime;

        Iterate();
    }
}

void GameManager::Iterate()
{
    uvec3 nextLocation = _gameMap.GetSnake()->GetNextHeadLocation();
    if (_grid.IsLocationValid(nextLocation))
    {
        _gameMap.GetSnake()->Move();
    }
    else
    {
        //SDL_Log("Snake has collided with a wall!");
    }

    _gameMap.CheckCollisions();

    Cell* headCell = _gameMap.GetSnakeCell();
    Cell* targetCell = _gameMap.GetTargetCell();
    _path = _pathfinder->FindPath(&_gameMap, headCell, targetCell);

    if (!_manualMovement)
    {
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
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            _mouseRotating = true;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            _mouseRotating = false;
        }
        break;
    case SDL_EVENT_MOUSE_MOTION:
        if (_mouseRotating)
        {
            _camera->ApplyMotion(event.motion.xrel, event.motion.yrel);
        }
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        _camera->AddRadius(event.wheel.y);
        break;
    case SDL_EVENT_KEY_DOWN:
        HandleScanCode(event.key.scancode);
        if (_manualMovement)
        {
            HandleSnakeMovementQE(event.key.scancode);
        }
        else
        {
            switch (event.key.scancode)
            {
            case SDL_SCANCODE_DOWN:
                _intervalLagMs += _intervalSum;
                break;
            case SDL_SCANCODE_UP:
                _intervalLagMs -= _intervalSum;
                if (_intervalLagMs <= 0)
                {
                    _intervalLagMs = 1;
                }
                break;
            }
        }
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void GameManager::HandleScanCode(SDL_Scancode scancode)
{
    switch (scancode)
    {
    case SDL_SCANCODE_R:
        srand(_seed);
        _gameMap.Reset();
        break;
    case SDL_SCANCODE_M:
        SetManualMovement(!_manualMovement);
        break;
    case SDL_SCANCODE_C:
        _renderer.SwapTheme();
        break;
    case SDL_SCANCODE_P:
        _isPaused = !_isPaused;
        break;
    case SDL_SCANCODE_RIGHT:
        if (_isPaused)
        {
            Iterate();
        }
        break;
    default: ;
    }
}

void GameManager::SetManualMovement(bool isManual)
{
    _manualMovement = isManual;
    if (_manualMovement)
    {
        _gameMap.GetSnake()->SetDirection(Directions::None);
        _currentGameStepIntervalMs = MANUAL_GAME_STEP_INTERVAL_MS;
    }
    else
    {
        _currentGameStepIntervalMs = AI_GAME_STEP_INTERVAL_MS;
    }
}

void GameManager::HandleSnakeMovementQE(SDL_Scancode scancode) const
{
    glm::vec3 dir;
    switch (scancode)
    {
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
        dir = _camera->GetForward();
        break;
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
        dir = -_camera->GetForward();
        break;
    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
        dir = _camera->GetRight();
        break;
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
        dir = -_camera->GetRight();
        break;
    case SDL_SCANCODE_Q:
        _gameMap.GetSnake()->SetDirection(Directions::Down);
        return;
    case SDL_SCANCODE_E:
        _gameMap.GetSnake()->SetDirection(Directions::Up);
        return;
    default:
        dir = Directions::None;
        return;
    }

    glm::ivec3 dominantDir = Directions::ToDominantHorizontalDirection(dir);

    //SDL_Log("dir: x=%f, y=%f, z=%f", dir.x, dir.y, dir.z);
    //SDL_Log("dominantDir: x=%d, y=%d, z=%d", dominantDir.x, dominantDir.y, dominantDir.z);

    _gameMap.GetSnake()->SetDirection(dominantDir);
}

void GameManager::Quit()
{
}

ImU32 GameManager::GetHUDColor()
{
    return _renderer.GetHUDColor();
}

void GameManager::RenderGame()
{
    glm::vec3 center = glm::vec3(_dimensions) * 0.5f;
    _camera->SetTarget(center);
    _camera->UpdatePosition();

    _renderer.RenderBackground();
    _renderer.RenderGrid(_grid);
    if (_manualMovement)
    {
        std::vector<Cell*> horizontalCells = _grid.GetCellsHorizontal(_gameMap.GetSnakeCell());
        _renderer.RenderHorizontalDirection(horizontalCells);

        std::vector<Cell*> verticalCells = _grid.GetCellsVertical(_gameMap.GetSnakeCell());
        _renderer.RenderHorizontalDirection(verticalCells);

        std::vector<Cell*> forwardDirectionCells = _grid.GetCellsLine(_gameMap.GetSnakeCell(),
                                                                      _gameMap.GetSnake()->GetDirection());
        _renderer.RenderForwardDirection(forwardDirectionCells);
    }

    _renderer.RenderGameMap(_gameMap);

    if (_path.IsValid())
    {
        _renderer.RenderPath(_path);
    }
}

void GameManager::RenderHUD()
{
    ImGui::Text("Score: %d", _score);
    ImGui::Text("Cells: %dx%dx%d ", _dimensions.x, _dimensions.y, _dimensions.z);

    ImGui::Text("M - manual movement");
    ImGui::Text("Hold LMB - rotate");
    ImGui::Text("Mouse wheel - zoom");

    if (_manualMovement)
    {
        ImGui::Text("WASD - horizontal");
        ImGui::Text("QE - vertical");
    }
    else
    {
        if (_isPaused)
        {
            ImGui::Text("Arrow up - Faster");
            ImGui::Text("Arrow down - Slower");
        }
    }

    ImGui::Text("P - pause");
    if (_isPaused)
    {
        ImGui::Text("Right arrow - One step");
    }

    ImGui::Text("C - Color theme");
    ImGui::Text("R - restart");
}