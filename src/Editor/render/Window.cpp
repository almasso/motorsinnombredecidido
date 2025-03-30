//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Window.h"
#include "imgui.h"

void editor::render::Window::render() {
    beforeRender();
    ImGui::Begin(_objectName.c_str(), nullptr, _windowFlags);
    onRender();
    ImGui::End();
}
