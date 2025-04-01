//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MainWindow.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include "render/WindowStack.h"

editor::render::windows::MainWindow::MainWindow(editor::Project *project) : Window("mainWindow"), _project(project) {
    editor::render::RenderManager::GetInstance().hideWindow();
    editor::render::RenderManager::GetInstance().setWidth(1920);
    editor::render::RenderManager::GetInstance().setHeight(1080);
    editor::render::RenderManager::GetInstance().enableWindowResizing();
    editor::render::RenderManager::GetInstance().centerWindowInScreen();
    editor::render::WindowStack::removeAllWindowsFromStack();
    editor::render::WindowStack::addWindowToStack(this);
    editor::render::RenderManager::GetInstance().showWindow();
}

void editor::render::windows::MainWindow::beforeRender() {

}

void editor::render::windows::MainWindow::onRender() {

}
