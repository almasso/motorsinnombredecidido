#include "TextureLoader.h"
#include "Color.h"

TextureLoader* TextureLoader::_instance = nullptr;

TextureLoader::TextureLoader(SDL_Renderer *renderer) : _renderer(renderer){
}

TextureLoader* TextureLoader::Init(SDL_Renderer* renderer)
{
    if (_instance == nullptr) {
        _instance = new TextureLoader(renderer);
        return _instance;
    }
    return nullptr;
}

Texture * TextureLoader::GetTexture(const Color& color) {
    if (_instance == nullptr) {
        return nullptr;
    }
    SDL_Renderer* renderer = _instance->_renderer;
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
    return texture;
}
