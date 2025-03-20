#include "RenderComponent.h"

bool RenderComponent::init() {
    _scene->registerRenderComponent(this, _layer);
    return true;
}
