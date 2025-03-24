#include "Rectangle.h"
#include "RenderManager.h"
#include "Transform.h"
#include <Core/ComponentData.h>

Rectangle::Rectangle(ComponentData const*data) : ComponentTemplate(data){
}

bool Rectangle::init() {
    _color = _data->getData<int>("color",0xFF0000FF);
    _size = _data->getVector("size",{1,1});
    return RenderComponent::init();
}

bool Rectangle::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    Vector2 size = _transform->getGlobalScale() * _size;
    Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
    return manager->drawRect(rectangle, _color);
}
