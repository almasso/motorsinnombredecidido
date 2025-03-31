//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Editor.h"
#include "render/RenderManager.h"
#include "io/InputManager.h"
#include "io/LocalizationManager.h"
#include "io/LuaManager.h"
#include "io/ProjectManager.h"
#include "render/Windows/WelcomeWindow.h"
#include "render/WindowStack.h"
#include "io/PreferencesManager.h"
#include "EditorError.h"
#include <SDL3/SDL.h>

std::unique_ptr<editor::Editor> editor::Editor::_instance = nullptr;

bool editor::Editor::Init() {
    editorAssert(_instance == nullptr, "Editor singleton instance is already initialized")
    _instance = std::unique_ptr<Editor>(new Editor());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

bool editor::Editor::init() {
    if(!io::LuaManager::Init()) return false;
    if(!io::PreferencesManager::Init()) return false;
    if(!io::LocalizationManager::Init()) return false;
    if(!render::RenderManager::Init(1210, 700)) return false;
    if(!io::InputManager::Init()) return false;
    if(!io::ProjectManager::Init()) return false;

    return true;
}

editor::Editor& editor::Editor::GetInstance() {
    editorAssert(_instance != nullptr, "Editor singleton instance is not initialized")
    return *_instance;
}

editor::Editor::~Editor() {
    io::ProjectManager::Dump();
    io::PreferencesManager::Dump();

    io::ProjectManager::Destroy();
    io::InputManager::Destroy();
    render::RenderManager::Destroy();
    io::LocalizationManager::Destroy();
    io::PreferencesManager::Destroy();
    io::LuaManager::Destroy();

    SDL_Quit();
}

void editor::Editor::mainLoop() {
    auto* ww = new render::windows::WelcomeWindow();
    bool noError = true;
    render::WindowStack::addWindowToStackFront(ww);
    while(!io::InputManager::GetInstance().quit() && noError) {
        io::InputManager::GetInstance().handleInput();
        noError = render::RenderManager::GetInstance().render();
    }
    delete ww;
}
