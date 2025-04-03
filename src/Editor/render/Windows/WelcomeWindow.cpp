//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WelcomeWindow.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "render/RenderManager.h"
#include "io/LocalizationManager.h"
#include "io/ProjectManager.h"
#include "common/Project.h"
#include "utils/IconsFontAwesome6.h"
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include "render/Modals/WelcomeWindow/RenameProjectModal.h"
#include "render/Modals/WelcomeWindow/DeleteProjectModal.h"
#include "render/Modals/WelcomeWindow/CreateProjectModal.h"
#include "render/WindowStack.h"
#include "render/Modals/WelcomeWindow/SettingsModal.h"
#include "render/Subwindows/WelcomeWindow/WelcomeWindowLeftPanel.h"
#include "render/Subwindows/WelcomeWindow/WelcomeWindowRightPanel.h"
#include "render/Subwindows/WelcomeWindow/WelcomeWindowProjectManagementButtons.h"

editor::render::windows::WelcomeWindow::WelcomeWindow() : Window("welcomeWindow") {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
          ImGuiWindowFlags_NoCollapse |
          ImGuiWindowFlags_NoScrollbar |
          ImGuiWindowFlags_NoScrollWithMouse |
          ImGuiWindowFlags_NoDocking;
    _windowFlags |= flags;
    RenderManager::GetInstance().setWindowName(io::LocalizationManager::GetInstance().getString("window.global.editorname") + " " + io::LocalizationManager::GetInstance().getString("window.global.currentversion"));

    // Añadimos la fuente Raleway
    ImFontConfig config;
    config.OversampleH = 5;
    config.OversampleV = 5;

    RenderManager::GetInstance().loadFont("Raleway 24", "settings/fonts/Raleway-Regular.ttf", 24.0f, &config);
    RenderManager::GetInstance().setDefaultFont(RenderManager::GetInstance().getFont("Raleway 24"));
    RenderManager::GetInstance().loadFont("Raleway 16", "settings/fonts/Raleway-Regular.ttf", 16.0f, &config);
    RenderManager::GetInstance().loadFont("Raleway 20", "settings/fonts/Raleway-Regular.ttf", 20.0f, &config);
    RenderManager::GetInstance().loadFont("Raleway 40", "settings/fonts/Raleway-Regular.ttf", 40.0f, &config);
    RenderManager::GetInstance().loadFont("Raleway 30", "settings/fonts/Raleway-Regular.ttf", 30.0f, &config);
    RenderManager::GetInstance().loadFont("Raleway 90", "settings/fonts/Raleway-Regular.ttf", 90.0f, &config);
    ImFontConfig iconConfig;
    iconConfig.MergeMode = true;
    iconConfig.PixelSnapH = true;
    const ImWchar icons[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    RenderManager::GetInstance().loadFont("FA 900", "settings/fonts/fa-solid-900.ttf", 48.0f, &iconConfig, icons);

    createModals();
    createSubwindows();
}

void editor::render::windows::WelcomeWindow::createModals() {
    for (Project *pr: io::ProjectManager::GetInstance().getProjects()) {
        if(!pr->isSetToDelete()) {
            _renameProjects[pr] = new editor::render::modals::RenameProjectModal(pr);
            WindowStack::addWindowToStack(_renameProjects[pr]);
            _deleteProjects[pr] = new editor::render::modals::DeleteProjectModal(pr);
            WindowStack::addWindowToStack(_deleteProjects[pr]);
        }
    }
    _createProject = new editor::render::modals::CreateProjectModal();
    WindowStack::addWindowToStack(_createProject);
    _settings = new editor::render::modals::SettingsModal();
    WindowStack::addWindowToStack(_settings);
}

void editor::render::windows::WelcomeWindow::createSubwindows() {
    _leftPanel = new editor::render::subwindows::WelcomeWindowLeftPanel(_settings);
    _rightPanel = new editor::render::subwindows::WelcomeWindowRightPanel(&_deleteProjects, &_renameProjects);
    _projectManagementButtons = new editor::render::subwindows::WelcomeWindowProjectManagementButtons(&_deleteProjects, &_renameProjects, _createProject);
}

editor::render::windows::WelcomeWindow::~WelcomeWindow() noexcept {
    for(auto _proj : _renameProjects) {
        delete _proj.second;
    }
    _renameProjects.clear();
    for(auto _proj : _deleteProjects) {
        delete _proj.second;
    }
    _deleteProjects.clear();
    delete _leftPanel;
    _leftPanel = nullptr;
    delete _rightPanel;
    _rightPanel = nullptr;
    delete _projectManagementButtons;
    _projectManagementButtons = nullptr;
    delete _createProject;
    _createProject = nullptr;
    delete _settings;
    _settings = nullptr;
}

void editor::render::windows::WelcomeWindow::beforeRender() {
    setFullscreen();
}

void editor::render::windows::WelcomeWindow::onRender() {
    drawWindow();
}

void editor::render::windows::WelcomeWindow::setFullscreen() {
    ImGui::SetNextWindowSize(ImVec2(RenderManager::GetInstance().getWidth(), RenderManager::GetInstance().getHeight()), ImGuiCond_Always);
    ImGui::SetWindowPos(ImVec2(0,0), ImGuiCond_Always);
}

void editor::render::windows::WelcomeWindow::drawWindow() {
    // Panel izquierdo (texto y botón de ajustes)
    _leftPanel->render();

    ImGui::SameLine();

    // Panel derecho (botones de acceso a los proyectos que haya cargados)
    _rightPanel->render();

    ImGui::SameLine();
    _projectManagementButtons->render();
}

