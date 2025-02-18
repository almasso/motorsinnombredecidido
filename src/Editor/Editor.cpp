//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Editor.h"
#include <cassert>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

Editor* Editor::_instance = nullptr;

Editor::~Editor() {

}

void Editor::Init() {
    assert(_instance == nullptr);
    _instance = new Editor();
    if(!_instance->init()) {
        delete _instance;
        _instance = nullptr;
    }
}

bool Editor::init() {
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_GetError(); //TODO: Lanzar una ventana de alerta indicando el error. No podemos lanzarla con SDL, puesto que no se habría iniciado.
        SDL_Quit();
        return false;
    }
}

Editor* Editor::GetInstance() {
    return _instance;
}