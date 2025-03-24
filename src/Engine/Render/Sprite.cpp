#include "Sprite.h"
#include "RenderManager.h"
#include "Texture.h"
#include <Load/ResourceHandler.h>

Sprite::Sprite(std::string const &path) : Resource(path), _rect() {
}

bool Sprite::load() {
  return true;
}

void Sprite::unload() {
}

const Texture* Sprite::getTexture() const{
  return ResourceHandler<Texture>::Instance()->get(_texturePath);
}

const Rect & Sprite::getRect() const{
  return _rect;
}
