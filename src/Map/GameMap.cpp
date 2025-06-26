#include "GameMap.h"
#include "Elements/Snake.h"

GameMap::GameMap(Grid* grid): _grid(grid), _snake(std::make_unique<Snake>())
{
}

GameMap::GameMap(const GameMap& other)
    : _grid(other._grid), _targetLocation(other._targetLocation)
{
    if (other._snake)
    {
        _snake = std::make_unique<Snake>(*other._snake);
    }
}

GameMap& GameMap::operator=(const GameMap& other)
{
    if (this != &other)
    {
        _grid = other._grid;
        _targetLocation = other._targetLocation;
        if (other._snake)
        {
            _snake = std::make_unique<Snake>(*other._snake);
        }
        else
        {
            _snake.reset();
        }
    }
    return *this;
}

Snake* GameMap::GetSnake() const
{
    return _snake.get();
}

Cell* GameMap::GetSnakeCell() const
{
    return _grid->GetCell(_snake->GetHeadLocation());
}

Cell* GameMap::GetTargetCell() const
{
    return _grid->GetCell(_targetLocation);
}

std::vector<Cell*> GameMap::GetFreeCells() const
{
    std::vector<Cell*> freeCells;
    for (Cell* const& cell : _grid->GetCells())
    {
        if (IsCellFree(cell))
        {
            freeCells.push_back(cell);
        }
    }

    return freeCells;
}

std::vector<Cell*> GameMap::GetNeighbors(Cell* cell) const
{
    std::vector<Cell*> cellNeighbors = _grid->GetNeighbors(cell);
    std::vector<Cell*> validNeighbors;

    for (Cell* neighbor : cellNeighbors)
    {
        if (IsCellFree(neighbor))
        {
            validNeighbors.push_back(neighbor);
        }
    }

    return validNeighbors;
}

Cell* GameMap::GetCell(uvec3 position) const
{
    return _grid->GetCell(position);
}

void GameMap::ResetTargetLocation()
{
    std::vector<uvec3> availablePositions;
    uvec3 dimensions = _grid->GetDimensions();

    for (unsigned int z = 0; z < dimensions.z; ++z)
    {
        for (unsigned int y = 0; y < dimensions.y; ++y)
        {
            for (unsigned int x = 0; x < dimensions.x; ++x)
            {
                uvec3 pos = {x, y, z};
                Cell* cell = _grid->GetCell(pos);
                if (cell && cell->GetType() == CellType::Empty && !_snake->IsAnyBodyPartInPosition(pos))
                {
                    availablePositions.push_back(pos);
                }
            }
        }
    }

    if (availablePositions.empty())
    {
        _targetLocation = uvec3{0, 0, 0};
    }

    int randomIndex = rand() % availablePositions.size();
    _targetLocation = availablePositions[randomIndex];
}

void GameMap::CheckCollisions()
{
    uvec3 headLocation = _snake->GetHeadLocation();
    if (headLocation == _targetLocation)
    {
        _snake->Grow();
        ResetTargetLocation();

        if (onSnakeReachedTarget)
        {
            onSnakeReachedTarget();
        }
    }

    /*if (_snake.CheckSelfCollision())
    {
        //TODO
    }*/
}

bool GameMap::IsCellFree(const Cell* cell) const
{
    return !_snake->IsAnyBodyPartInPosition(cell->GetGridPosition());
}

void GameMap::Reset()
{
    _snake->Reset();
    ResetTargetLocation();
}