#include <SDL3/SDL.h>
#include "RenderManager.h"

#include <iostream>
#include <ostream>

RenderManager::RenderManager() {
    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error al inicializar SDL: (%s)", SDL_GetError());
        }
    }
    if (!SDL_CreateWindowAndRenderer("Game", 1920, 1080, SDL_WINDOW_MAXIMIZED, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error al crear la ventana: (%s)", SDL_GetError());
    }
}

void RenderManager::present() const {
    SDL_RenderPresent(renderer);
}

void RenderManager::clear() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void RenderManager::drawRect(const SDL_FRect rect, const int color) const {
    const int r = color >> 16, g = color >> 8 , b = color;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderManager::getWindowSize(int *width, int *height) const {
    SDL_GetWindowSize(window,width,height);
}

void RenderManager::checkExit(bool *exit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_FINGER_UP)
            *exit = true;
    }
}

void RenderManager::exit() {
    SDL_Quit();
}
