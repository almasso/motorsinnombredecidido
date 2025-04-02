#include "ComponentFactory.h"

#include <Audio/AudioSource.h>
#include <Collisions/Collider.h>
#include <Gameplay/LocalVariables.h>
#include <Render/Transform.h>
#include <Render/Rectangle.h>
#include <Render/Animator.h>
#include <Render/SpriteRenderer.h>
#include <Render/Camera.h>
#include <Gameplay/Events/EventHandler.h>
#include <Gameplay/Movement/MovementManager.h>
#include <Gameplay/Movement/MovementObstacle.h>
#include <Gameplay/Movement/MovementComponent.h>
#include <Gameplay/Movement/PlayerInput.h>

#include "ComponentData.h"

ComponentFactory::ComponentFactory() :
    _numComponents(0) {
    registerComponent<Transform>();
    registerComponent<Rectangle>();
    registerComponent<Animator>();
    registerComponent<SpriteRenderer>();
    registerComponent<Camera>();
    registerComponent<AudioSource>();
    registerComponent<Collider>();
    registerComponent<EventHandler>();
    registerComponent<PlayerInput>();
    registerComponent<MovementManager>();
    registerComponent<MovementObstacle>();
    registerComponent<MovementComponent>();
    registerComponent<LocalVariables>();
}

Component* ComponentFactory::createComponent(ComponentData const* data) {
    if (auto finder = _factory.find(data->getId()); finder != _factory.end()) {
        return finder->second(data);
    }
    return nullptr;
}