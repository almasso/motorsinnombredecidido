#include "SpriteRenderer.h"
#include "RenderManager.h"
#include "Transform.h"
#include <Core/ComponentData.h>
#include <Load/ResourceHandler.h>

SpriteRenderer::SpriteRenderer(ComponentData const*data) : ComponentTemplate(data) {
}

bool SpriteRenderer::init() {
    if (!setSprite(_data->getData<std::string>("sprite")))
        return false;
    return RenderComponent::init();
}

bool SpriteRenderer::render(RenderManager *manager) {
    const Sprite* sprite  = ResourceHandler<Sprite>::Instance()->get(_sprite);
    if (!sprite) {return false;}
    Vector2 position = _transform->getGlobalPosition();
    Vector2 size = _transform->getGlobalScale() * _size;
    Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
    return manager->drawSprite(rectangle, sprite, _transform->getGlobalRotation());
}

bool SpriteRenderer::setSprite(const std::string& spritePath) {
    const Sprite* sprite = ResourceHandler<Sprite>::Instance()->get(spritePath);
    if (!sprite) {return false;}
    _sprite = spritePath;
    _size = Vector2(sprite->getRect().w, sprite->getRect().h);
    return true;
}
