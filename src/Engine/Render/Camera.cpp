#include "Camera.h"
#include "RenderManager.h"
#include "Transform.h"
#include <Core/ComponentData.h>

Camera::Camera(ComponentData *data) : ComponentTemplate(data) {
}

bool Camera::init() {
    _size = _data->getVector("scale",{1,1});
    return RenderComponent::init();
}

bool Camera::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    manager->setViewRect(position, _size);
    return true;
}
