#include "Node.h"

void Node::UpdateCosts(float costFomStart, float costToEnd)
{
    _costFromStart = costFomStart;
    _costToEnd = costToEnd;
    _totalCost = _costFromStart + _costToEnd;
}