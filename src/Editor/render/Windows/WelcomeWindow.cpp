//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WelcomeWindow.h"
#include <imgui.h>
#include "render/RenderManager.h"
#include "io/LocalizationManager.h"

editor::render::windows::WelcomeWindow::WelcomeWindow() : Window("welcomeWindow") {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
          ImGuiWindowFlags_NoCollapse |
          ImGuiWindowFlags_NoScrollbar |
          ImGuiWindowFlags_NoScrollWithMouse;
    _windowFlags |= flags;
    _elements.emplace_back(setFullScreen);
}

void editor::render::windows::WelcomeWindow::setFullScreen() {
    ImGui::SetNextWindowSize(ImVec2(RenderManager::GetInstance().getWidth(), RenderManager::GetInstance().getHeight()), ImGuiCond_Always);
    ImGui::SetWindowPos(ImVec2(0,0), ImGuiCond_Always);
}

void editor::render::windows::WelcomeWindow::drawButton() {

}
