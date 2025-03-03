//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Editor.h"
#include <cassert>
#include "render/RenderManager.h"
#include "io/InputManager.h"
#include "io/ResourceManager.h"
#include "io/LocalizationManager.h"
#include "render/Windows/WelcomeWindow.h"
#include "render/WindowStack.h"

std::unique_ptr<editor::Editor> editor::Editor::_instance = nullptr;

editor::Editor::~Editor() {
    delete _renderManager;
    _renderManager = nullptr;
    delete _inputManager;
    _inputManager = nullptr;
}

bool editor::Editor::Init() {
    assert(_instance == nullptr && "Editor singleton instance is already initialized || La instancia del singleton del editor ya está inicializada");
    _instance = std::unique_ptr<Editor>(new Editor());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

bool editor::Editor::init() {
    _renderManager = new render::RenderManager();
    if(!_renderManager->init(1010, 650)) return false;
    _inputManager = new io::InputManager();
    if(!_inputManager->init()) return false;
    if(!io::ResourceManager::Init()) return false;
    if(!io::LocalizationManager::Init()) return false;

    return true;
}

editor::Editor& editor::Editor::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

void editor::Editor::mainLoop() {
    render::WindowStack::addWindowToStack(new render::windows::WelcomeWindow());
    while(!_inputManager->quit()) {
        _inputManager->handleInput();
        _renderManager->render();
    }
}
