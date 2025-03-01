//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WindowStack.h"
#include "UIElement/Window.h"

SearchableList<editor::render::ui_element::Window*> editor::render::WindowStack::_windowStack;

void editor::render::WindowStack::addWindowToStack(ui_element::Window *window) {
    _windowStack.push_back(window);
}

void editor::render::WindowStack::removeWindowFromStack(ui_element::Window *window) {
    _windowStack.erase(window);
}

void editor::render::WindowStack::removeAllWindowsFromStack() {
    for(ui_element::Window* window : _windowStack) {
        removeWindowFromStack(window);
    }
}

editor::render::ui_element::Window* editor::render::WindowStack::getWindowByName(const std::string& name) {
    for (ui_element::Window* window : _windowStack) {
        if(window->getName() == name) return window;
    }
    return nullptr;
}
