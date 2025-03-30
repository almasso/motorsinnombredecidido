//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WindowStack.h"
#include "RenderObject.h"

SearchableList<editor::render::RenderObject*> editor::render::WindowStack::_windowStack;

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
    for(RenderObject* window : _windowStack) {
        removeWindowFromStack(window);
    }
}

editor::render::RenderObject* editor::render::WindowStack::_getWindowByName(const std::string& name) {
    for (RenderObject* window : _windowStack) {
        if(window->getName() == name) return window;
    }
    return nullptr;
}

void editor::render::WindowStack::renderWindows() {
    for(RenderObject* window : _windowStack) window->render();
}
