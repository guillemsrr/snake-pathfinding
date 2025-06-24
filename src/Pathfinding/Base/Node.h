#pragma once

class Cell;

class Node
{
public:
    Cell* cell; // Pointer to the actual cell in your grid
    Node* parent; // To reconstruct the path
    float gCost; // Cost from the start node to this node
    float hCost; // Heuristic cost from this node to the end node
    float fCost; // Total cost (gCost + hCost)

    Node(Cell* c, Node* p = nullptr) : cell(c), parent(p), gCost(0.0f), hCost(0.0f), fCost(0.0f)
    {
    }

    // Overload comparison operators for use in priority queue
    // (A Node with a lower fCost is "greater" for a min-priority queue)
    bool operator>(const Node& other) const
    {
        return fCost > other.fCost;
    }
};