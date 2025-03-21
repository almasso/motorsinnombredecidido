#include "Entity.h"
#include "Component.h"

bool Entity::update() {
     for (auto [id,component] : _components) {
          if (component->isEnabled() && !component->update()) {
               return false;
          }
     }
     for (Entity* child : _children) {
          if (child->isActive() && !child->update()) {
               return false;
          }
     }
     return true;
}

bool Entity::fixedUpdate() {
     for (auto [id,component] : _components) {
          if (component->isEnabled() && !component->fixedUpdate()) {
               return false;
          }
     }
     for (Entity* child : _children) {
          if (!child->fixedUpdate()) {
               return false;
          }
     }
     return true;
}

bool Entity::isActive() const {
     return _active && (_parent == nullptr || _parent->isActive());
}

void Entity::setActive(const bool active) {
     if (active != _active) {
          _active = active;
          for (auto [id,component] : _components) {
               if (component->isEnabled()) {
                    if(active) component->onEnable();
                    else component->onDisable();
               }
          }
          for (Entity* child : _children) {
               if (child->isActive()) {
                    for (auto [id,component] : child->_components) {
                         if (component->isEnabled()) {
                              if(active) component->onEnable();
                              else component->onDisable();
                         }
                    }
               }
          }
     }
}

bool Entity::isAlive() const {
     return _alive;
}

void Entity::destroy() {
     _alive = false;
}

Entity * Entity::getParent() const {
     return _parent;
}