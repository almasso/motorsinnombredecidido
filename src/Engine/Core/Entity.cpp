#include "Entity.h"

bool Entity::update() {
     return false;
}

bool Entity::fixedUpdate() {
     return false;
}

bool Entity::render(RenderManager *manager) {
     return false;
}

bool Entity::isEnabled() {
     return false;
}

void Entity::setEnabled() {
}

Component * Entity::getComponent() {
  return nullptr;
}

Component * Entity::addComponent(Component *component) {
     return nullptr;
}

void Entity::removeComponent(Component *component) {
}