// Copyright (c) Guillem Serra. All Rights Reserved.

#include "Cell.h"

Cell::Cell() : _type(CellType::Empty) {}

CellType Cell::GetType() const
{
    return _type;
}

void Cell::SetType(CellType type)
{
    _type = type;
}