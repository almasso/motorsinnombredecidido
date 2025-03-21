#include "Rectangle.h"
#include "RenderManager.h"
#include "Transform.h"
#include "Utils/RPGError.h"

const std::string Rectangle::id = "Rectangle";

bool Rectangle::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    Vector2 size = _transform->getGlobalScale() * _size;
    Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
    return manager->drawRect(rectangle, _color);
}
