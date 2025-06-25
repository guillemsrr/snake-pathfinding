#include "Path.h"

void Path::Add(Cell* cell)
{
    Cells.push_back(cell);
}

std::vector<Cell*>::iterator Path::GetFirst()
{
    return Cells.begin();
}

std::vector<Cell*>::iterator Path::GetLast()
{
    return Cells.end(); 
}

Cell* Path::GetSecond() const
{
    return Cells[1];
}

bool Path::IsValid() const
{
    return Cells.size() > 1;
}