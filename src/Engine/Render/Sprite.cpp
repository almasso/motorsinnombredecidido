#include "Sprite.h"
#include "RenderManager.h"

Sprite::Sprite(std::string const &path) : Resource(path) {
}

bool Sprite::load() {
  return true;
}

void Sprite::unload() {
}

Texture* Sprite::getTexture() const{
  return nullptr;
}

const Rect & Sprite::getRect() const{
  return _rect;
}
