#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Collider.h"

class CollisionManager {
private:
    std::unordered_set<Collider*> _colliders;

    static CollisionManager* _instance;

    void handleCollision(Collider* collider, Collider* collider2);
    void handleCollisionEnd(Collider* collider, Collider* collider2);
    void updateCollisions(Collider* collider, Collider* collider2);

public:
    ~CollisionManager();
    static void Init();
    static CollisionManager* Instance();
    static void Shutdown();
    void fixedUpdate();
    void registerCollider(Collider* collider);
    void unregisterCollider(Collider* collider);
};

#endif //COLLISIONMANAGER_H
