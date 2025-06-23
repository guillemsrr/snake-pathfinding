// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

enum class CellType
{
    Empty,
    Wall
};

class Cell
{
public:
    Cell();
    
    CellType GetType() const;
    void SetType(CellType type);

private:
    CellType _type;
};