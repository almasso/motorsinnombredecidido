#ifndef COLLIDER_H
#define COLLIDER_H

#include <unordered_set>
#include <Core/ComponentTemplate.h>
#include <Utils/Rect.h>
#include <Utils/Vector2.h>

class CollisionManager;
class Transform;

class Collider : public ComponentTemplate<"Collider"> {
private:
    friend CollisionManager;

    Vector2 _pos;
    Vector2 _size;

    Transform const* _transform;

    std::unordered_set<Collider*> _isColliding;
    std::unordered_set<Collider*> _justCollided;
    std::unordered_set<Collider*> _collisionEnded;

public:
    Collider(ComponentData const* data);
    ~Collider() override;
    bool init() override;
    void onEnable() override;
    void onDisable() override;
    bool isCollidingWith(Collider* other) const;
    bool justCollidedWith(Collider* other) const;
    bool collisionEndedWith(Collider* other) const;
    Rect getRect() const;
};



#endif //COLLIDER_H
