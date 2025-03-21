#include "SpriteRenderer.h"
#include "RenderManager.h"
#include "Transform.h"

bool SpriteRenderer::render(RenderManager *manager) {
    Vector2 position = _transform->getGlobalPosition();
    Vector2 size = _transform->getGlobalScale() * _size;
    Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
    return manager->drawSprite(rectangle, _sprite, _transform->getGlobalRotation());
}

void SpriteRenderer::setSprite(const Sprite *sprite) {
    _sprite = sprite;
    _size = Vector2(_sprite->rect.w, _sprite->rect.h);
}
