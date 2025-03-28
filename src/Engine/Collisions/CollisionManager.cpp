#include "CollisionManager.h"

#include <cassert>
#include <SDL3/SDL_rect.h>

CollisionManager* CollisionManager::_instance = nullptr;

void CollisionManager::handleCollision(Collider* collider, Collider* collider2) {
    if (collider->_isColliding.insert(collider2).second)
        collider->_justCollided.insert(collider2);
}

void CollisionManager::handleCollisionEnd(Collider* collider, Collider* collider2) {
    if (auto it = collider->_isColliding.find(collider2);
        it == collider->_isColliding.end()) {
        collider->_collisionEnded.insert(collider2);
        collider->_isColliding.erase(it);
    }
}

void CollisionManager::updateCollisions(Collider* collider, Collider* collider2) {
    Rect colliderRect = collider->getRect();
    Rect collider2Rect = collider2->getRect();
    if (SDL_HasRectIntersectionFloat(&colliderRect, &collider2Rect))
        handleCollision(collider, collider2);
    else
        handleCollisionEnd(collider, collider2);
}

CollisionManager::CollisionManager() = default;

CollisionManager::~CollisionManager() {
    _colliders.clear();
}

void CollisionManager::Init() {
    assert(_instance == nullptr);
    _instance = new CollisionManager();
}

CollisionManager* CollisionManager::Instance() {
    return _instance;
}

void CollisionManager::Shutdown() {
    delete _instance;
}


void CollisionManager::fixedUpdate() {
    for (auto& collider : _colliders) {
        collider->_justCollided.clear();
        collider->_collisionEnded.clear();
        for (auto& collider2 : _colliders) {
            updateCollisions(collider, collider2);
        }
    }
}

void CollisionManager::registerCollider(Collider* collider) {
    _colliders.insert(collider);
}

void CollisionManager::unregisterCollider(Collider* collider) {
    _colliders.erase(collider);
}
