#include "Camera.h"
#include "RenderManager.h"
#include "Transform.h"

Camera::Camera(ComponentData *data) : ComponentTemplate(data) {
}

bool Camera::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    manager->setViewRect(position, _size);
    return true;
}
