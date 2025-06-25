#pragma once

class Cell;

class Node
{
public:
    Node(Cell* c, Node* p = nullptr) : cell(c), parent(p), _costFromStart(0.f), _costToEnd(0.f), _totalCost(0.f)
    {
    }

    Cell* cell;
    Node* parent;

    float GetCostFromStart() const
    {
        return _costFromStart;
    }

    float GetTotalCost() const
    {
        return _totalCost;
    }

    void UpdateCosts(float costFomStart, float costToEnd);

private:
    float _costFromStart;
    float _costToEnd;
    float _totalCost;
};