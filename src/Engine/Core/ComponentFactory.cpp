#include "ComponentFactory.h"
#include "ComponentData.h"
#include <Audio/AudioSource.h>
#include <Collisions/Collider.h>
#include <Render/Transform.h>
#include <Render/Rectangle.h>
#include <Render/Animator.h>
#include <Render/SpriteRenderer.h>
#include <Render/Camera.h>
#include <Render/Text.h>
#include <Input/Button.h>
#include <Gameplay/Events/EventHandler.h>
#include <Gameplay/Movement/MovementManager.h>
#include <Gameplay/Movement/MovementObstacle.h>
#include <Gameplay/Movement/MovementComponent.h>
#include <Gameplay/Movement/PlayerInput.h>
#include <Gameplay/Dialog/TextBox.h>
#include <Gameplay/Events/LocalVariables.h>
#include <Utils/Error.h>

ComponentFactory::ComponentFactory() :
    _numComponents(0) {
    registerComponent<Transform>();
    registerComponent<Rectangle>();
    registerComponent<Animator>();
    registerComponent<SpriteRenderer>();
    registerComponent<Camera>();
    registerComponent<Text>();
    registerComponent<Button>();
    registerComponent<AudioSource>();
    registerComponent<Collider>();
    registerComponent<EventHandler>();
    registerComponent<PlayerInput>();
    registerComponent<MovementManager>();
    registerComponent<MovementObstacle>();
    registerComponent<MovementComponent>();
    registerComponent<LocalVariables>();
    registerComponent<TextBox>();
}

Component* ComponentFactory::createComponent(ComponentData const* data) {
    if (auto finder = _factory.find(data->getId()); finder != _factory.end()) {
        return finder->second(data);
    }
    Error::ShowError("No se encontro el componente " + data->getId(), "Todos los componentes deben delcararse con el metodo RegisterComponent de Component Factory");
    return nullptr;
}