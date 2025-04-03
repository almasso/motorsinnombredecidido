#include "Camera.h"
#include "RenderManager.h"
#include "Transform.h"
#include <Core/ComponentData.h>

Camera::Camera(ComponentData const*data) : ComponentTemplate(data) {
}

bool Camera::init() {
    _size = _data->getVector("size",{1280 ,720});
    if (!RenderComponent::init()) return false;
    _layer = -1;
    return true;
}

bool Camera::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    auto [screenScale, screenOffset] = manager->setViewRect(position - _size/2.f, _size);
    _screenScale = screenScale;
    _screenOffset = screenOffset;
    return true;
}

Vector2 Camera::screenToWorld(const Vector2& screenPos) const {
    Vector2 position = _transform->getGlobalPosition();
    return (screenPos - _screenOffset) / _screenScale + (position - _size / 2.0f);
}
