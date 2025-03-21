#include "SpriteRenderer.h"
#include "RenderManager.h"
#include "Transform.h"

bool SpriteRenderer::render(RenderManager *manager) {
    Sprite *sprite = new Sprite(_sprite);
    Vector2 position = _transform->getGlobalPosition();
    Vector2 size = _transform->getGlobalScale() * _size;
    Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
    return manager->drawSprite(rectangle, sprite, _transform->getGlobalRotation());
}

void SpriteRenderer::setSprite(const std::string& spritePath) {
    _sprite = spritePath;
    Sprite* sprite = new Sprite(_sprite);
    _size = Vector2(sprite->getRect().w, sprite->getRect().h);
}
