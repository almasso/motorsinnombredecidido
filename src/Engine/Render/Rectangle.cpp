#include "Rectangle.h"
#include "RenderManager.h"
#include "Transform.h"

const std::string Rectangle::id = "Rectangle";

bool Rectangle::render(RenderManager *manager) {
    if (Transform* transform = _entity->getComponent<Transform>(); transform != nullptr) {
        Vector2 position = transform->getGlobalPosition();
        Vector2 size = transform->getGlobalScale() * _size;
        Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
        manager->drawRect(rectangle, _color);
        return true;
    }
    return false;
}
