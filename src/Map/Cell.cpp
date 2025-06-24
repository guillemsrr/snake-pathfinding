// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Cell.h"

Cell::Cell(uvec3 position): _type(CellType::Empty)
{
    GridPosition = position;
}

CellType Cell::GetType() const
{
    return _type;
}

void Cell::SetType(CellType type)
{
    _type = type;
}