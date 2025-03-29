#include "MovementManager.h"
#include <cmath>
#include <queue>
#include <unordered_map>
#include <vector>

MovementManager::MovementManager(ComponentData const *data) :
    ComponentTemplate(data),
    _squareW(0),
    _squareH(0),
    _occupiedPositions() {
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
    int x = std::round(position.getX() / _squareW) * _squareW;
    int y = std::round(position.getY() / _squareH) * _squareH;
    return Vector2(x, y);
}

std::vector<Vector2> MovementManager::calculatePath(const Vector2 &position, const Vector2 &target) const {
    std::vector<Vector2> path;
    Vector2 start = getCell(position);
    Vector2 end = getCell(target);

    std::queue<Vector2> openList;
    std::unordered_map<Vector2, Vector2> cameFrom;

    openList.push(start);
    cameFrom[start] = start;

    std::vector<Vector2> directions = {{_squareW,0}, {-_squareW,0}, {0,_squareH}, {0,-_squareH}};

    while (!openList.empty()) {
        Vector2 current = openList.front();
        openList.pop();

        if (current == end) {
            Vector2 step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[step];
            }
            std::ranges::reverse(path);
            return path;
        }

        for (const Vector2& dir : directions) {
            Vector2 neighbor = current + dir;
            if (isOccupied(neighbor) || cameFrom.contains(neighbor)) continue;

            openList.push(neighbor);
            cameFrom[neighbor] = current;
        }
    }
    return path;
}
