#include "InputManager.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <Utils/RPGError.h>

InputState InputManager::input_state = InputState();

bool InputManager::init() {
    if (!SDL_InitSubSystem(SDL_INIT_EVENTS)) {
        {
            RPGError::ShowError("Error al inicializar SDL_EVENTS", SDL_GetError());
            return false;
        }
    }
    return true;
}

void InputManager::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                input_state.exit = true;
                break;
            default: ;
        }
    }
}

void InputManager::shutdown() {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

const InputState & InputManager::getState() {
    return input_state;
}

