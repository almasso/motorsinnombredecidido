#include "InputManager.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <Utils/RPGError.h>

InputManager* InputManager::instance = nullptr;

InputManager::InputManager() = default;

InputManager* InputManager::Init() {
    if (instance == nullptr) {
        if (!SDL_InitSubSystem(SDL_INIT_EVENTS)) {
            {
                RPGError::ShowError("Error al inicializar SDL_EVENTS", SDL_GetError());
                return instance;
            }
        }
        return instance = new InputManager();
    }
    RPGError::ShowError("Error al inicializar InputManager", "Ya existia una instancia de InputManager");
    return nullptr;
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

void InputManager::shutdown() const {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    delete this;
}

const InputState & InputManager::GetState() {
    return instance->input_state;
}
