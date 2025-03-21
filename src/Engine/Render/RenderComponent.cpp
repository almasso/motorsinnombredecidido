#include "RenderComponent.h"
#include <Utils/RPGError.h>

bool RenderComponent::init() {
    _scene->registerRenderComponent(this, _layer);
    _transform = _entity->getComponent<Transform>();
    if (Transform* transform = _entity->getComponent<Transform>(); transform != nullptr) {
        RPGError::ShowError("Render object sin transform", "Todos los RenderObjects requieren de un componente Transform para funcionar");
        return false;
    }
    return Component::init();
}

bool RenderComponent::OnEnable