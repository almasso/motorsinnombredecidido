#include "SpriteRenderer.h"
#include "RenderManager.h"
#include "Transform.h"
#include <Load/ResourceHandler.h>

SpriteRenderer::SpriteRenderer(ComponentData *data) : ComponentTemplate(data) {
}

bool SpriteRenderer::render(RenderManager *manager) {
    const Sprite* sprite  = ResourceHandler<Sprite>::Instance()->get(_sprite);
    Vector2 position = _transform->getGlobalPosition();
    Vector2 size = _transform->getGlobalScale() * _size;
    Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
    return manager->drawSprite(rectangle, sprite, _transform->getGlobalRotation());
}

void SpriteRenderer::setSprite(const std::string& spritePath) {
    _sprite = spritePath;
    const Sprite* sprite = ResourceHandler<Sprite>::Instance()->get(_sprite);
    _size = Vector2(sprite->getRect().w, sprite->getRect().h);
}
