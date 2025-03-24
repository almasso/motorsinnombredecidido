#include "ComponentFactory.h"

#include <Audio/AudioSource.h>
#include <Collisions/Collider.h>
#include <Render/Transform.h>
#include <Render/Rectangle.h>
#include <Render/Animator.h>
#include <Render/SpriteRenderer.h>
#include <Render/Camera.h>

#include "ComponentData.h"

ComponentFactory::ComponentFactory() : _numComponents(0) {
    registerComponent<Transform>();
    registerComponent<Rectangle>();
    registerComponent<Animator>();
    registerComponent<SpriteRenderer>();
    registerComponent<Camera>();
    registerComponent<AudioSource>();
    registerComponent<Collider>();
}

Component* ComponentFactory::createComponent(ComponentData const* data) {
    if (auto finder = _factory.find(data->getId()); finder != _factory.end()) {
        return finder->second(data);
    }
    return nullptr;
}