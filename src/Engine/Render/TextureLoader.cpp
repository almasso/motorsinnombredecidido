#include "TextureLoader.h"
#include "Color.h"
#include <Utils/RPGError.h>


bool TextureLoader::Init(SDL_Renderer* renderer)
{
    if (!_renderer) {
        _renderer = renderer;
        return true;
    }
    return false;
}

Texture* TextureLoader::GetTexture(const Color& color) {
    if (!_renderer) {
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_SetRenderTarget(_renderer, texture);
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(_renderer);
    SDL_SetRenderTarget(_renderer, nullptr);
    return texture;
}

Texture* TextureLoader::GetTexture(const char *filePath) {
    if (!_renderer) {
        return nullptr;
    }
    //SDL_Surface* surface = IMG_Load(filePath);
    SDL_Surface* surface = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        RPGError::ShowError("Error al cargar imagen", SDL_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        RPGError::ShowError("Error al crear textura", SDL_GetError());
    }

    return texture;
}
