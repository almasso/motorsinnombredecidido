#include "InputManager.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <Utils/RPGError.h>


InputManager::InputManager() {
}

bool InputManager::init() {
    if (!SDL_InitSubSystem(SDL_INIT_EVENTS)) {
        {
            RPGError::ShowError("Error al inicializar SDL_EVENTS", SDL_GetError());
            return false;
        }
    }
    return true;
}

bool InputManager::checkExit() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_FINGER_UP || event.type == SDL_EVENT_QUIT) {
            return true;
        }
    }
    return false;
}

void InputManager::shutdown() const {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

