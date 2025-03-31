#include "RenderManager.h"
#include "Texture.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_init.h>
#include <Utils/RPGError.h>

#include "TextureLoader.h"

RenderManager::RenderManager() : _screenScale(0), _window(nullptr), _renderer(nullptr), _width(0), _height(0) {
}

bool RenderManager::init(const int& width, const int& height) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        {
            RPGError::ShowError("Error al inicializar SDL_VIDEO", SDL_GetError());
            return false;
        }
    }
    if (!SDL_CreateWindowAndRenderer("Game", width, height, SDL_WINDOW_MAXIMIZED, &_window, &_renderer))
    {
        RPGError::ShowError("Error al crear la ventana", SDL_GetError());
        return false;
    }
    int w, h;
    getWindowSize(&w, &h);
    _width = static_cast<float>(w);
    _height = static_cast<float>(h);
    _viewOffset = {w/2.f,h/2.f};
    _screenOffset = {0,0};
    _screenScale = 1;
    return TextureLoader::Init(_renderer);
}

void RenderManager::present() const {
    SDL_RenderPresent(_renderer);
}

void RenderManager::clear() const {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

bool RenderManager::drawRect(const Rect &rect, const Color& color) const {
    SDL_SetRenderDrawColor(_renderer, color.r, color.r, color.b, color.a);
    return SDL_RenderFillRect(_renderer, &rect);
}

bool RenderManager::drawSprite(const Rect &rect, const Sprite *sprite, float rotation) const {
    Rect drawRect = convertRect(rect);
    if (drawRect.w == 0 || drawRect.h == 0) {
        return true;
    }
    return SDL_RenderTextureRotated(_renderer, sprite->getTexture()->texture, &sprite->getRect(), &drawRect, rotation, nullptr, SDL_FLIP_NONE);
}

void RenderManager::getWindowSize(int *width, int *height) const {
    SDL_GetWindowSize(_window,width,height);
}

void RenderManager::shutdown() const {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

std::pair<float, const Vector2&> RenderManager::setViewRect(const Vector2 &viewPosition, const Vector2 &viewSize)  {
    _viewOffset = viewPosition;
    float tempScaleX = _width / viewSize.getX();
    float tempScaleY = _height / viewSize.getY();
    _screenScale = std::min(tempScaleY, tempScaleX);
    _screenOffset = Vector2(_width - viewSize.getX() * _screenScale, _height - viewSize.getY() * _screenScale)/2.f;
    return std::pair<float, Vector2&>(_screenScale, _screenOffset);
}

Rect RenderManager::convertRect(const Rect &rect) const {
    Rect tempRect = {
        (rect.x - _viewOffset.getX()) * _screenScale + _screenOffset.getX(),
        (rect.y - _viewOffset.getY()) * _screenScale + _screenOffset.getY(),
        rect.w * _screenScale,
        rect.h * _screenScale
    };
    if (tempRect.x >= _width || tempRect.x < -tempRect.w || tempRect.y >= _height || tempRect.y < -tempRect.h)
        return {0,0,0,0};
    return tempRect;
}
