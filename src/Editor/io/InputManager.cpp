//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "InputManager.h"
#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>
#include <cassert>

std::unique_ptr<editor::io::InputManager> editor::io::InputManager::_instance = nullptr;

bool editor::io::InputManager::Init() {
    assert(_instance == nullptr && "Input manager singleton instance is already initialized || La instancia del singleton del gestor de input ya está inicializada");
    _instance = std::unique_ptr<InputManager>(new InputManager());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

bool editor::io::InputManager::init() {
    if(!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
        SDL_GetError();
        return false;
    }
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    return true;
}

editor::io::InputManager &editor::io::InputManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::io::InputManager::~InputManager() {
    SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
    SDL_Quit();
}

void editor::io::InputManager::handleInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if(event.type == SDL_EVENT_QUIT)
            _quitSignal = true;
    }
}

bool editor::io::InputManager::quit() const {
    return _quitSignal;
}
