//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WindowStack.h"
#include "Window.h"

SearchableList<editor::render::Window*> editor::render::WindowStack::_windowStack;

void editor::render::WindowStack::addWindowToStack(Window *window) {
    _windowStack.push_back(window);
}

void editor::render::WindowStack::removeWindowFromStack(Window *window) {
    _windowStack.erase(window);
}

void editor::render::WindowStack::removeAllWindowsFromStack() {
    for(Window* window : _windowStack) {
        removeWindowFromStack(window);
    }
}

editor::render::Window* editor::render::WindowStack::getWindowByName(const std::string& name) {
    for (Window* window : _windowStack) {
        if(window->getName() == name) return window;
    }
    return nullptr;
}

void editor::render::WindowStack::renderWindows() {
    for(Window* window : _windowStack) window->render();
}
