#include "ComponentFactory.h"
#include <Render/Transform.h>
#include <Render/Rectangle.h>
#include <Render/Animator.h>
#include <Render/SpriteRenderer.h>
#include <Render/Camera.h>

ComponentFactory::ComponentFactory() : _numComponents(0) {
    registerComponent<Transform>();
    registerComponent<Rectangle>();
    registerComponent<Animator>();
    registerComponent<SpriteRenderer>();
    registerComponent<Camera>();
}

Component * ComponentFactory::createComponent(const std::string &name) {
    if (auto finder = _factory.find(name); finder != _factory.end()) {
        return finder->second();
    }
    return nullptr;
}