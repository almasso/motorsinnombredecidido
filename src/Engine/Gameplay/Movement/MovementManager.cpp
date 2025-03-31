#include "MovementManager.h"
#include <Core/ComponentData.h>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <vector>

MovementManager::MovementManager(ComponentData const *data) :
    ComponentTemplate(data),
    _tileWidth(0),
    _tileHeight(0) {
}

bool MovementManager::init() {
    _tileWidth = _data->getData<int>("tileWidth",1);
    _tileHeight = _data->getData<int>("tileHeight",1);;
    return true;
}

std::unordered_set<Vector2>::iterator MovementManager::registerObstacle(const Vector2 &position) {
    return _occupiedPositions.insert(position).first;
}

void MovementManager::unregisterObstacle(std::unordered_set<Vector2>::iterator iter) {
    _occupiedPositions.erase(iter);
}

bool MovementManager::isOccupied(const Vector2 &position) const {
    return _occupiedPositions.contains(position);
}

Vector2 MovementManager::getCell(const Vector2 &position) const {
    int x = std::round(position.getX() / _tileWidth) * _tileWidth;
    int y = std::round(position.getY() / _tileHeight) * _tileHeight;
    return Vector2(x, y);
}

std::vector<Vector2> MovementManager::calculatePath(const Vector2 &position, const Vector2 &target) const {
    std::vector<Vector2> path;
    Vector2 start = getCell(position);
    Vector2 end = getCell(target);

    struct Node {
        Vector2 pos;
        float cost;
        bool operator>(const Node& other) const { return cost > other.cost; }
    };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::unordered_map<Vector2, Vector2> cameFrom;
    std::unordered_map<Vector2, float> costSoFar;

    openList.push({start, 0});
    cameFrom[start] = start;
    costSoFar[start] = 0;

    std::vector<Vector2> directions = {{_tileWidth, 0}, {-_tileWidth, 0}, {0, _tileHeight}, {0, -_tileHeight}};

    while (!openList.empty()) {
        Vector2 current = openList.top().pos;
        openList.pop();

        if (current == end) {
            Vector2 step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[step];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const Vector2& dir : directions) {
            Vector2 neighbor = current + dir;
            if (isOccupied(neighbor)) continue;

            float newCost = costSoFar[current] + 1;
            if (!costSoFar.contains(neighbor) || newCost < costSoFar[neighbor]) {
                costSoFar[neighbor] = newCost;
                openList.push({neighbor, newCost + neighbor.distanceToManhattan(end)});
                cameFrom[neighbor] = current;
            }
        }
    }
    return path;
}
