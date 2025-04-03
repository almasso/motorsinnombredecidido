#include "RenderComponent.h"

#include <Core/ComponentData.h>

#include "Transform.h"
#include <Core/Entity.h>
#include <Core/Scene.h>
#include <Utils/Error.h>

RenderComponent::RenderComponent(ComponentData const* data): Component(data), _layer(0) {
}

bool RenderComponent::init() {
    _layer = _data->getData<int>("layer",0);
    _transform = _entity->getComponent<Transform>();
    if (_transform == nullptr) {
        Error::ShowError("Render object sin transform", "Todos los RenderObjects requieren de un componente Transform para funcionar");
        return false;
    }
    return true;
}

void RenderComponent::onEnable() {
    _scene->registerRenderComponent(this, _layer);
}

void RenderComponent::onDisable() {
    _scene->unregisterRenderComponent(this, _layer);
}
