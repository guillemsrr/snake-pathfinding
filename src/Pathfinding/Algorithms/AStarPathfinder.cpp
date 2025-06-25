#include "AStarPathfinder.h"
#include <queue>
#include <unordered_map>

#include "Map/Cell.h"

#include "Pathfinding/Base/IGraph.h"
#include "Pathfinding/Base/Node.h"
#include "Pathfinding/Base/Path.h"

Path AStarPathfinder::FindPath(IGraph& graph, Cell* start, Cell* end)
{
    if (!start || !end)
    {
        return {};
    }

    auto cmp = [](Node* a, Node* b)
    {
        return a->GetTotalCost() > b->GetTotalCost();
    };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openSet(cmp);

    std::unordered_map<Cell*, Node*> allNodes;
    std::unordered_map<Cell*, float> cellsCost;
    std::unordered_map<Cell*, Cell*> cameFrom;

    Node* startNode = new Node(start);
    float heuristicCost = CalculateHeuristic(start->GetGridPosition(), end->GetGridPosition());
    startNode->UpdateCosts(0, heuristicCost);
    openSet.push(startNode);
    allNodes[start] = startNode;
    cellsCost[start] = 0;

    while (!openSet.empty())
    {
        Node* currentNode = openSet.top();
        openSet.pop();

        if (currentNode->cell == end)
        {
            Path path = GetPath(startNode, currentNode);
            for (auto const& [cell_ptr, node_ptr] : allNodes)
            {
                delete node_ptr;
            }
            return path;
        }

        for (Cell* neighborCell : graph.GetNeighbors(currentNode->cell))
        {
            float cost = CalculateHeuristic(currentNode->cell->GetGridPosition(), neighborCell->GetGridPosition());
            float costFomStart = currentNode->GetCostFromStart() + cost;

            if (cellsCost.find(neighborCell) == cellsCost.end() || costFomStart < cellsCost[neighborCell])
            {
                cameFrom[neighborCell] = currentNode->cell;
                cellsCost[neighborCell] = costFomStart;

                Node* neighborNode;
                if (allNodes.find(neighborCell) == allNodes.end())
                {
                    neighborNode = new Node(neighborCell, currentNode);
                    allNodes[neighborCell] = neighborNode;
                }
                else
                {
                    neighborNode = allNodes[neighborCell];
                    neighborNode->parent = currentNode;
                }

                float costToEnd = CalculateHeuristic(neighborCell->GetGridPosition(), end->GetGridPosition());
                neighborNode->UpdateCosts(costFomStart, costToEnd);

                openSet.push(neighborNode);
            }
        }
    }

    for (auto const& [cell_ptr, node_ptr] : allNodes)
    {
        delete node_ptr;
    }
    return {};
}

float AStarPathfinder::CalculateHeuristic(const vec3 a, const vec3 b) const
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

Path AStarPathfinder::GetPath(Node* start, Node* current) const
{
    Path totalPath;
    while (current != start)
    {
        totalPath.Add(current->cell);
        current = current->parent;
    }
    totalPath.Add(start->cell);
    std::reverse(totalPath.GetFirst(), totalPath.GetLast());
    return totalPath;
}