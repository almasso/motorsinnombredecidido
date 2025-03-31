#ifndef MOVEMENTMANAGER_H
#define MOVEMENTMANAGER_H
#include <Core/ComponentTemplate.h>
#include <Utils/Vector2.h>
#include <unordered_set>
#include <vector>


class MovementManager : public ComponentTemplate<MovementManager, "MovementManager"> {
    private:
    float _squareW, _squareH;
    std::unordered_set<Vector2> _occupiedPositions;
    public:
    explicit MovementManager(ComponentData const *data);
    std::unordered_set<Vector2>::iterator registerObstacle(const Vector2 &position);
    void unregisterObstacle(std::unordered_set<Vector2>::iterator iter);
    bool isOccupied(const Vector2 &position) const;
    Vector2 getCell(const Vector2 &position) const;
    std::vector<Vector2> calculatePath(const Vector2 &position, const Vector2 &target) const;
};



#endif //MOVEMENTMANAGER_H
