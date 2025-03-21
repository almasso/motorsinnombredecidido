#include "Camera.h"
#include "RenderManager.h"
#include "Transform.h"
#include "Utils/RPGError.h"

bool Camera::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    manager->setViewRect(position, _size);
    return true;
}
