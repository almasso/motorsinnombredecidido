#include "Entity.h"

#include <sol/state.hpp>

#include "Component.h"

Entity::Entity(): _parent(nullptr), _active(false), _alive(true) {
}

bool Entity::init() {
     for (auto [id,component] : _components) {
          if (component->init() && _active) {
               component->initEnable();
          }
          else {
               return false;
          }
     }
     for (Entity* child : _children) {
          if (!child->init()) {
               return false;
          }
     }
     return true;
}

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
     setActive(false);
}

bool Entity::addChild(Entity* child) {
     if (_children.insert(child).second) {
          child->setParent(this);
          return true;
     }
     return false;
}

void Entity::removeChild(Entity *child) {
     _children.erase(child);
}

void Entity::setParent(Entity* parent) {
     _parent = parent;
}

Entity * Entity::getParent() const {
     return _parent;
}

bool Entity::addComponent(Component *component, const std::string& id) {
     if (_components.insert({component->getOrder(), component}).second) {
          if (_componentIds.insert({id, component}).second) {
               return true;
          }
     }
     return false;
}

Component* Entity::getComponent(const std::string& componentID) {
     auto finder = _componentIds.find(componentID);
     if (finder == _componentIds.end()) {
          return nullptr;
     }
     return finder->second;
}

void Entity::RegisterToLua(sol::state& lua) {
     sol::usertype<Entity> type = lua.new_usertype<Entity>("Entity");
     type["destroy"] = &Entity::destroy;
}
