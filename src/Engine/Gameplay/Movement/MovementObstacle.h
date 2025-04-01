#ifndef MOVEMENTOBSTACLE_H
#define MOVEMENTOBSTACLE_H
#include <Core/ComponentTemplate.h>
#include <Utils/Vector2.h>
#include <unordered_set>

class Transform;
class MovementManager;

class ComponentClass(MovementObstacle) {
    protected:
        Transform* _transform;
        MovementManager* _manager;
        std::unordered_set<Vector2>::iterator _iterator;
    public:
        MovementObstacle(const ComponentData* data);
        bool init() override;
        void onEnable() override;
        void onDisable() override;
};


#endif //MOVEMENTOBSTACLE_H
