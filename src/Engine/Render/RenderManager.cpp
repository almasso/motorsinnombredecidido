#include "RenderManager.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_init.h>
#include <Utils/RPGError.h>



RenderManager::RenderManager() : window(nullptr), renderer(nullptr) {}

bool RenderManager::init(const int& width, const int& height) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        {
            RPGError::ShowError("Error al inicializar SDL_VIDEO", SDL_GetError());
            return false;
        }
    }
    if (!SDL_CreateWindowAndRenderer("Game", width, height, SDL_WINDOW_MAXIMIZED, &window, &renderer))
    {
        RPGError::ShowError("Error al crear la ventana", SDL_GetError());
        return false;
    }
    return true;
}

void RenderManager::present() const {
    SDL_RenderPresent(renderer);
}

void RenderManager::clear() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void RenderManager::drawRect(const Rect &rect, const int &color) const {
    const int r = color >> 16, g = color >> 8 , b = color;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderManager::getWindowSize(int *width, int *height) const {
    SDL_GetWindowSize(window,width,height);
}

void RenderManager::shutdown() const {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
