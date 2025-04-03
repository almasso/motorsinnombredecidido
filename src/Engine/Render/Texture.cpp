#include "Texture.h"
#include "TextureLoader.h"
#include "SDL3/SDL_render.h"

Texture::Texture(std::string const &path) : Resource(path), texture(nullptr){
}

bool Texture::load() {
  texture = TextureLoader::GetTexture(_path);
  _size = texture->w * texture->h;
  return texture;
}

void Texture::unload() {
  SDL_DestroyTexture(texture);
}
