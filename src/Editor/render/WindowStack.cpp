//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WindowStack.h"
#include "RenderObject.h"
#include "RenderManager.h"

SearchableList<editor::render::RenderObject*> editor::render::WindowStack::_windowStack;
bool editor::render::WindowStack::_stopRendering = false;

void editor::render::WindowStack::addWindowToStack(RenderObject *window) {
    _windowStack.push_back(window);
}

void editor::render::WindowStack::addWindowToStackFront(RenderObject *window) {
    _windowStack.push_front(window);
}

void editor::render::WindowStack::removeWindowFromStack(RenderObject *window) {
    _windowStack.erase(window);
}

void editor::render::WindowStack::removeAllWindowsFromStack() {
    _stopRendering = true;
    _windowStack.clear();
}

editor::render::RenderObject* editor::render::WindowStack::_getWindowByName(const std::string& name) {
    for (RenderObject* window : _windowStack) {
        if(window->getName() == name) return window;
    }
    return nullptr;
}

void editor::render::WindowStack::renderWindows() {
    for(auto it = _windowStack.begin(); it != _windowStack.end();) {
        if(RenderManager::GetInstance().isWindowShown() && !_stopRendering) {
            (*it)->render();
            ++it;
        }
        else {
            _stopRendering = false;
            return;
        }
    }
}
