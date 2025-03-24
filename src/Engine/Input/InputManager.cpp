#include "InputManager.h"

#include <Render/RenderManager.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <Utils/RPGError.h>

InputManager* InputManager::_instance = nullptr;

InputManager::InputManager() = default;

InputManager* InputManager::Init() {
    if (_instance == nullptr) {
        if (!SDL_InitSubSystem(SDL_INIT_EVENTS)) {
            {
                RPGError::ShowError("Error al inicializar SDL_EVENTS", SDL_GetError());
                return _instance;
            }
        }
        return _instance = new InputManager();
    }
    RPGError::ShowError("Error al inicializar InputManager", "Ya existia una instancia de InputManager");
    return nullptr;
}

void InputManager::update(const int& width, const int& height) {
    _inputState.mouse_down = false;
    _inputState.mouse_up = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                _inputState.exit = true;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                _inputState.mouse_x = event.motion.x;
                _inputState.mouse_y = event.motion.y;
            break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    _inputState.mouse_down = true;
                    _inputState.mouse_pressed = true;
                }
            break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    _inputState.mouse_up = true;
                    _inputState.mouse_pressed = false;
                }
            break;

            case SDL_EVENT_FINGER_DOWN:
                _inputState.mouse_x = event.tfinger.x * width;
                _inputState.mouse_y = event.tfinger.y * height;
                _inputState.mouse_down = true;
                _inputState.mouse_pressed = true;
            break;

            case SDL_EVENT_FINGER_UP:
                _inputState.mouse_x = event.tfinger.x * width;
                _inputState.mouse_y = event.tfinger.y * height;
                _inputState.mouse_up = true;
                _inputState.mouse_pressed = false;
            break;

            case SDL_EVENT_FINGER_MOTION:
                _inputState.mouse_x = event.tfinger.x * width;
                _inputState.mouse_y = event.tfinger.y * height;
            break;

            default:
                break;
        }
    }
}

void InputManager::shutdown() const {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    delete this;
}

const InputState & InputManager::GetState() {
    return _instance->_inputState;
}
