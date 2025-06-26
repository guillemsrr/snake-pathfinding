#include "SafeSnakePathfinder.h"
#include <queue>
#include <unordered_set>
#include "AStarPathfinder.h"

#include "Elements/Snake.h"

#include "Map/GameMap.h"

#include "Pathfinding/Base/Path.h"

#include <SDL3/SDL_log.h>

SafeSnakePathfinder::~SafeSnakePathfinder()
{
    delete _aStarPathdinder;
}

Path SafeSnakePathfinder::FindPath(IGraph* graph, Cell* start, Cell* end)
{
    Path pathToFood = _aStarPathdinder->FindPath(graph, start, end);

    GameMap* gameMapGraph = dynamic_cast<GameMap*>(graph);

    if (pathToFood.IsValid())
    {
        if (!IsPathTrappable(gameMapGraph, pathToFood))
        {
            return pathToFood;
        }
    }
    else
    {
        SDL_Log("Invalid path to food, Game over");
    }

    for (Cell* neighbor : gameMapGraph->GetNeighbors(start))
    {
        Path detour = _aStarPathdinder->FindPath(graph, neighbor, end);
        if (detour.IsValid() && !IsPathTrappable(gameMapGraph, detour))
        {
            // Move toward neighbor (wasting a step)
            Path singleStepPath;
            singleStepPath.Cells.push_back(start);
            singleStepPath.Cells.push_back(neighbor);
            return singleStepPath;
        }
    }

    std::vector<Cell*> neighbors = gameMapGraph->GetNeighbors(start);
    if (!neighbors.empty())
    {
        Cell* neighbor = neighbors[rand() % neighbors.size()];

        Path manhattahnPath;
        manhattahnPath.Cells.push_back(start);
        manhattahnPath.Cells.push_back(neighbor);
        if (manhattahnPath.IsValid() && !IsPathTrappable(gameMapGraph, manhattahnPath))
        {
            return manhattahnPath;
        }
    }

    // Find the safest cell with longest path
    Path longestPath = FindLongestSafePath(graph, start, gameMapGraph);
    return longestPath;
}

bool SafeSnakePathfinder::IsPathTrappable(GameMap* gameMap, const Path& path)
{
    GameMap simulatedGameMap = *gameMap;
    SimulateSnakeFollowPath(simulatedGameMap, path);
    Cell* headCell = simulatedGameMap.GetSnakeCell();
    Cell* tailCell = simulatedGameMap.GetCell(simulatedGameMap.GetSnake()->GetTailLocation());

    Path pathToTail = _aStarPathdinder->FindPath(&simulatedGameMap, headCell, tailCell);
    if (pathToTail.IsValid())
    {
        return false;
    }

    int numberCellsFlood = FloodFillAccessibleCellCount(simulatedGameMap, headCell);
    if (numberCellsFlood > simulatedGameMap.GetSnake()->GetLength())
    {
        return false;
    }

    return true;
}

void SafeSnakePathfinder::SimulateSnakeFollowPath(const GameMap& gameMap, const Path& path)
{
    Snake* simulatedSnake = gameMap.GetSnake();

    for (size_t i = 1; i < path.Cells.size(); ++i)
    {
        uvec3 direction = path.Cells[i]->GetGridPosition() - path.Cells[i - 1]->GetGridPosition();
        simulatedSnake->SetDirection(direction);
        simulatedSnake->Move();
    }
}

int SafeSnakePathfinder::FloodFillAccessibleCellCount(const GameMap& gameMap, Cell* start)
{
    if (!start)
    {
        return 0;
    }

    std::queue<Cell*> open;
    std::unordered_set<Cell*> visited;

    open.push(start);
    visited.insert(start);

    int count = 0;

    while (!open.empty())
    {
        Cell* current = open.front();
        open.pop();
        ++count;

        for (Cell* neighbor : gameMap.GetNeighbors(current))
        {
            if (visited.contains(neighbor))
            {
                continue;
            }

            visited.insert(neighbor);
            open.push(neighbor);
        }
    }

    return count;
}

Path SafeSnakePathfinder::FindLongestSafePath(IGraph* graph, Cell* start, GameMap* gameMapGraph)
{
    Path longestPath;
    std::vector<Cell*> freeCells = gameMapGraph->GetFreeCells();
    for (Cell* freeCell : freeCells)
    {
        Path currentPath = _aStarPathdinder->FindPath(graph, start, freeCell);
        if (currentPath.IsValid() && currentPath.Cells.size() > longestPath.Cells.size())
        {
            longestPath = currentPath;
        }
    }

    return longestPath;
}