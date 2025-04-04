//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MainWindow.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include "render/WindowStack.h"
#include "render/WindowItems/MapEditor.h"

editor::render::windows::MainWindow::MainWindow(editor::Project *project) : Window("mainWindow"), _project(project) {
    editor::render::RenderManager::GetInstance().hideWindow();
    editor::render::RenderManager::GetInstance().setWidth(1920);
    editor::render::RenderManager::GetInstance().setHeight(1080);
    editor::render::RenderManager::GetInstance().enableWindowResizing();
    editor::render::RenderManager::GetInstance().centerWindowInScreen();
    editor::render::RenderManager::GetInstance().setWindowName(editor::render::RenderManager::GetInstance().getWindowName() + " - " + _project->getName());
    editor::render::WindowStack::removeAllWindowsFromStack();
    editor::render::WindowStack::addWindowToStack(this);
    editor::render::RenderManager::GetInstance().showWindow();
    _mapEditor = new tabs::MapEditor(_project);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoScrollWithMouse |
                             ImGuiWindowFlags_NoDocking;
    _windowFlags |= flags;
}

void editor::render::windows::MainWindow::beforeRender() {
    ImGui::SetNextWindowSize(ImVec2(RenderManager::GetInstance().getWidth(), RenderManager::GetInstance().getHeight()), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
}

void editor::render::windows::MainWindow::onRender() {
    ImGui::BeginTabBar("tabBar");
    _mapEditor->render();
    ImGui::EndTabBar();
}
