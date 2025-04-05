#include "TextureLoader.h"
#include "Color.h"
#include <Utils/Error.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

SDL_Renderer* TextureLoader::_renderer = nullptr;

bool TextureLoader::Init(SDL_Renderer* renderer)
{
    if (!_renderer) {
        _renderer = renderer;
        return true;
    }
    return false;
}

SDL_Texture* TextureLoader::GetTexture(const Color& color) {
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

SDL_Texture* TextureLoader::GetTexture(const std::string& filePath) {
    return GetTexture(IMG_Load(filePath.c_str()));
}

SDL_Texture * TextureLoader::GetTexture(SDL_Surface *surface) {
    if (!_renderer || !surface) {
        Error::ShowError("Error al cargar imagen", SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        Error::ShowError("Error al crear textura", SDL_GetError());
    }
    return texture;
}

void TextureLoader::ResizeTexture(SDL_Texture*& texture, int width, int height, bool centered) {
    if (!_renderer) return;
    SDL_Texture* newTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!newTexture) {
        Error::ShowError("Error al crear textura vacía", SDL_GetError());
        return;
    }
    SDL_SetRenderTarget(_renderer, newTexture);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);
    SDL_FRect srcRect = {0, 0, static_cast<float>(std::min(texture->w, width)), static_cast<float>(std::min(texture->h, height))};
    SDL_FRect dstRect = {0, 0, srcRect.w, srcRect.h};
    if (centered) {
        dstRect.x = (width - dstRect.w) / 2.0f;
        dstRect.y = (height - dstRect.h) / 2.0f;
    }
    SDL_RenderTexture(_renderer, texture, &srcRect, &dstRect);
    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_DestroyTexture(texture);
    texture = newTexture;
}
