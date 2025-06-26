#include "SafeSnakePathfinder.h"
#include "AStarPathfinder.h"

#include "Elements/Snake.h"

#include "Map/GameMap.h"

#include "Pathfinding/Base/Path.h"

SafeSnakePathfinder::~SafeSnakePathfinder()
{
    delete _aStarPathdinder;
}

Path SafeSnakePathfinder::FindPath(IGraph* graph, Cell* start, Cell* end)
{
    Path pathToFood = _aStarPathdinder->FindPath(graph, start, end);

    GameMap* gameMapGraph = dynamic_cast<GameMap*>(graph);

    //check the if it has a correct exit simulating having arrived to target
    if (pathToFood.IsValid())
    {
        GameMap simulatedGameMap = *gameMapGraph;

        SimulateSnakeFollowPath(simulatedGameMap, pathToFood);
        Cell* headCell = simulatedGameMap.GetSnakeCell();
        Cell* tailCell = simulatedGameMap.GetCell(simulatedGameMap.GetSnake()->GetTailLocation());
        Path pathToTail = _aStarPathdinder->FindPath(&simulatedGameMap, headCell, tailCell);

        if (pathToTail.IsValid())
        {
            return pathToFood;
        }

        //Try reaching any open cell
        std::vector<Cell*> freeCells = simulatedGameMap.GetFreeCells();
        for (Cell* freeCell : freeCells)
        {
            Path altPath = _aStarPathdinder->FindPath(&simulatedGameMap, simulatedGameMap.GetSnakeCell(), freeCell);
            if (altPath.IsValid())
            {
                return pathToFood; // Still safe
            }
        }
    }

    // Find the safest cell with longest path
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

    //TODO: simulate that after having arrived there, it wont' get stuck

    return longestPath;
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