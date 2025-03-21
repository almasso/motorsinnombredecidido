#include "Camera.h"
#include "RenderManager.h"
#include "Transform.h"
#include "Utils/RPGError.h"

const std::string Camera::id = "Camera";

bool Camera::render(RenderManager *manager) {
    if (Transform* transform = _entity->getComponent<Transform>(); transform != nullptr) {
        Vector2 position = transform->getGlobalPosition();
        manager->setViewRect(position, _size);
        return true;
    }
    RPGError::ShowError("Render object sin transform", "El RenderObject de tipo Camera no tenía Transform");
    return false;
}
