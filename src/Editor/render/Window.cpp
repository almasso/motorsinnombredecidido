//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Window.h"
#include "imgui.h"

const std::string & editor::render::Window::getName() const {
    return _windowName;
}

void editor::render::Window::render() {
    beforeRender();
    ImGui::Begin(_windowName.c_str(), nullptr, _windowFlags);
    onRender();
    ImGui::End();
}
