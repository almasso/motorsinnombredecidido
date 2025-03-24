#include "Camera.h"
#include "RenderManager.h"
#include "Transform.h"
#include <Core/ComponentData.h>

Camera::Camera(ComponentData const*data) : ComponentTemplate(data) {
}

bool Camera::init() {
    _layer = -1;
    _size = _data->getVector("size",{1280 ,720});
    return RenderComponent::init();
}

bool Camera::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    manager->setViewRect(position - _size/2.f, _size);
    return true;
}
