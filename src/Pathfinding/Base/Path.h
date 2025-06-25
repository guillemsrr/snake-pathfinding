#pragma once
#include <vector>

class Cell;

struct Path
{
    void Add(Cell* cell);
    std::vector<Cell*>::iterator GetFirst();
    std::vector<Cell*>::iterator GetLast();
    Cell* GetSecond() const;

    bool IsValid() const;

    std::vector<Cell*> Cells;
};