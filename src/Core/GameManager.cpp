// Copyright (c) Guillem Serra. All Rights Reserved.

#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include "GameConfig.h"

constexpr uint64_t GAME_STEP_INTERVAL_MS = 100; // Update game logic every 100ms

GameManager::GameManager()
    : _window(nullptr), _renderer(nullptr), _snake(GAME_WIDTH / 2, GAME_HEIGHT / 2), _grid(GAME_WIDTH, GAME_HEIGHT),
      // Start in a "not playing" state until Init is called
      _lastGameStepTime(0)
{
    srand(time(nullptr));
}

GameManager::~GameManager()
{
}

bool GameManager::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    _window = window;
    _renderer = renderer;

    //_snake = Snake(GAME_WIDTH / 2, GAME_HEIGHT / 2);
    //_grid = Grid(GAME_WIDTH / 2, GAME_HEIGHT / 2);
    _target.Respawn();
    _lastGameStepTime = SDL_GetTicks();
    return true;
}

void GameManager::Iterate(uint64_t currentTime)
{
    if (currentTime > _lastGameStepTime + GAME_STEP_INTERVAL_MS)
    {
        _snake.Move();
        CheckCollisions();
        _lastGameStepTime = currentTime;
    }

    RenderGame();
}

SDL_AppResult GameManager::HandleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        SnakeDirection dir;
        bool directionChanged = true;

        switch (event.type)
        {
        case SDLK_UP: dir = SnakeDirection::Up;
            break;
        case SDLK_DOWN: dir = SnakeDirection::Down;
            break;
        case SDLK_LEFT: dir = SnakeDirection::Left;
            break;
        case SDLK_RIGHT: dir = SnakeDirection::Right;
            break;
        default:
            directionChanged = false;
            break;
        }

        if (directionChanged)
        {
            _snake.SetDirection(dir);
        }
    }

    return SDL_APP_CONTINUE;
}

void GameManager::Quit()
{
    // Perform cleanup here if necessary
}

void GameManager::CheckCollisions()
{
    // Check for collision with target
    if (_snake.Position == _target.Position)
    {
        _snake.Grow();
        _target.Respawn();
    }

    // Check for collision with self
    if (_snake.CheckSelfCollision())
    {
        //TODO
    }
}

void GameManager::RenderGame()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    //Render grid
    SDL_SetRenderDrawColor(_renderer, 128, 128, 128, 255);

    // Draw vertical lines
    for (int x = 0; x <= GAME_WIDTH; x++)
    {
        SDL_FPoint start = {x * 20.0f, 0.0f};
        SDL_FPoint end = {x * 20.0f, GAME_HEIGHT * 20.0f};
        SDL_RenderLine(_renderer, start.x, start.y, end.x, end.y);
    }

    // Draw horizontal lines
    for (int y = 0; y <= GAME_HEIGHT; y++)
    {
        SDL_FPoint start = {0.0f, y * 20.0f};
        SDL_FPoint end = {GAME_WIDTH * 20.0f, y * 20.0f};
        SDL_RenderLine(_renderer, start.x, start.y, end.x, end.y);
    }

    // Render Snake
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
    for (const auto& segment : _snake.GetBody())
    {
        SDL_FRect rect = {(float)segment.x * 20, (float)segment.y * 20, 20, 20};
        SDL_RenderFillRect(_renderer, &rect);
    }

    // Render Target
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
    SDL_FRect rect = {(float)_target.Position.x * 20, (float)_target.Position.y * 20, 20, 20};
    SDL_RenderFillRect(_renderer, &rect);

    SDL_RenderPresent(_renderer);
}