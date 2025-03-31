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
#include "render/Modals/RenameProjectModal.h"
#include "render/Modals/DeleteProjectModal.h"
#include "render/Modals/CreateProjectModal.h"
#include "render/WindowStack.h"
#include "render/Modals/SettingsModal.h"

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
    delete _createProject;
    _createProject = nullptr;
    delete _settings;
    _settings = nullptr;
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
    float leftPanelWidth = RenderManager::GetInstance().getWidth() / 2 - 90;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    float windowHeight = ImGui::GetContentRegionAvail().y;
    float rightPanelWidth = windowWidth - leftPanelWidth - 200;

    // Panel izquierdo (texto y botón de ajustes)
    ImGui::BeginChild("LP", ImVec2(leftPanelWidth, windowHeight), false);
    {
        ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 90"));
        ImGui::Text(io::LocalizationManager::GetInstance().getString("window.global.editorname").c_str());
        ImGui::PopFont();
        ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 40"));
        ImGui::Text(io::LocalizationManager::GetInstance().getString("window.global.currentversion").c_str());
        ImGui::PopFont();

        ImGui::SetCursorPosY(windowHeight - 100);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 1)); // Fondo del botón
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1)); // Al pasar el ratón por encima
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1)); // Al pulsar el botón
        ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
        if (ImGui::Button(ICON_FA_GEAR, ImVec2(100, 100))) {
            _settings->show();
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine(0, 20);

        ImGui::BeginChild("Authors");
        {
            ImGui::Text(io::LocalizationManager::GetInstance().getString("author.authors").c_str());
            ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 20"));
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                               io::LocalizationManager::GetInstance().getString("author.almasso").c_str());
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                               io::LocalizationManager::GetInstance().getString("author.algonz39").c_str());
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                               io::LocalizationManager::GetInstance().getString("author.miguelcga").c_str());
            ImGui::PopFont();
        }
        ImGui::EndChild();

    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Panel derecho (botones de acceso a los proyectos que haya cargados)
    ImGui::BeginChild("RP", ImVec2(rightPanelWidth, windowHeight), true);
    {
        ImGui::PushItemWidth(300);
        // Barra de búsqueda
        ImGuiTextFilter filter;

        char *buf = filter.InputBuf;
        if (ImGui::InputTextWithHint("##filterProjects", io::LocalizationManager::GetInstance().getString(
                "window.welcomewindow.searchbar").c_str(), buf, IM_ARRAYSIZE(filter.InputBuf))) {
            filter.Build();
        }

        ImGui::SameLine();
        if (ImGui::Button("X")) {
            filter.Clear();
        }

        ImGui::PopItemWidth();

        ImGui::Spacing();

        // Lista de botones con los proyectos
        ImGui::BeginChild("PL", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
        {
            for (Project *pr: io::ProjectManager::GetInstance().getProjects()) {
                if(!pr->isSetToDelete()) {
                    if (filter.PassFilter(pr->getName().c_str()) || filter.PassFilter(pr->getPath().string().c_str())) {
                        if (pr != *io::ProjectManager::GetInstance().getProjects().begin()) {
                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();
                        }
                        drawProjectButton(pr);
                    }
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("PCDB");
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 1)); // Fondo del botón
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1)); // Al pasar el ratón por encima
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1)); // Al pulsar el botón
        ImGui::SetCursorPosY(windowHeight / 2 - 150);

        ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
        if(ImGui::Button("##ButtonCreateProject", ImVec2(200, 150))) {
            _showCreateProject = true;
        }
        ImVec2 buttonPos = ImGui::GetItemRectMin();
        ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 10, buttonPos.y - 20));
        ImGui::Text(ICON_FA_FOLDER_PLUS);
        ImGui::PopFont();

        ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 60, buttonPos.y + 60));
        ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 30"));
        ImGui::TextWrapped(io::LocalizationManager::GetInstance().getString("action.newproject").c_str());
        ImGui::PopFont();

        ImGui::NewLine();

        ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
        if(ImGui::Button("##ButtonOpenProject", ImVec2(200, 150))) {
            std::string route = searchProject();
            if(!route.empty()) io::ProjectManager::GetInstance().addProject(route);
        }
        buttonPos = ImGui::GetItemRectMin();
        ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 10, buttonPos.y - 20));
        ImGui::Text(ICON_FA_FOLDER_OPEN);
        ImGui::PopFont();

        ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 60, buttonPos.y + 60));
        ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 30"));
        ImGui::TextWrapped(io::LocalizationManager::GetInstance().getString("action.openproject").c_str());
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
    }
    ImGui::EndChild();
}

void editor::render::windows::WelcomeWindow::drawProjectButton(Project* project) {
    const std::string& projectRoute = project->getPath().string();
    const std::string& projectName = project->getName();
    const std::tm& lastModified = project->getLastModificationTime();

    ImVec2 buttonSize = ImVec2(-1, 120);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 1)); // Fondo del botón
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1)); // Al pasar el ratón por encima
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1)); // Al pulsar el botón

    ImGui::BeginDisabled(!project->isFound());
    if(ImGui::Button(std::string("##But" + projectRoute).c_str() , buttonSize)) {
        // Abrir la ventana principal del editor
    }
    ImGui::EndDisabled();

    if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !project->isFound()) {
        ImGui::SetTooltip(io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.projectnotfound").c_str());
    }

    if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup((projectRoute + "Options").c_str());
    }


    ImGui::PopStyleColor(3);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - buttonSize.y);

    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 40"));
    ImGui::Text(projectName.c_str());
    ImGui::PopFont();

    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 20"));
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), projectRoute.c_str());
    ImGui::PopFont();

    std::ostringstream oss;
    oss << std::put_time(&lastModified, "%d/%m/%Y %H:%M:%S");
    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 16"));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - buttonSize.y + 145);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), std::string(io::LocalizationManager::GetInstance().getString("window.welcomewindow.lastmodified") + " "
    + oss.str()).c_str());
    ImGui::PopFont();

    if (ImGui::BeginPopupContextItem((projectRoute + "Options").c_str())) {
        ImGui::BeginDisabled(project->isFound());
        if(ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.relocateproject").c_str())) {
            std::string route = searchProject();
            std::filesystem::path p(route);
            if(!route.empty()) project->updatePath(p.parent_path().string());
        }
        ImGui::EndDisabled();
        ImGui::BeginDisabled(!project->isFound());
        if (ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.renameproject").c_str())) {
            _showRenameProject = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        if (ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.deleteproject").c_str())) {
            _showDeleteConfirmation = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor();
        ImGui::EndPopup();
    }

    if(_showDeleteConfirmation) {
        _deleteProjects[project]->show();
        if(_deleteProjects[project]->hasConfirmedDeletion()) {
            delete _deleteProjects[project];
            _deleteProjects.erase(project);
            delete _renameProjects[project];
            _renameProjects.erase(project);
        }
        if(!_deleteProjects[project]->isOpen()) _showDeleteConfirmation = false;
    }
    if(_showRenameProject) {
        _renameProjects[project]->show();
        if(!_renameProjects[project]->isOpen()) _showRenameProject = false;
    }
    if(_showCreateProject) {
        _createProject->show();
        if(!_createProject->isOpen()) _showCreateProject = false;

    }

    editor::Project* createdProj = _createProject->getCreatedProject();
    if(createdProj != nullptr) {
        io::ProjectManager::GetInstance().addProject(createdProj);
        _renameProjects[createdProj] = new editor::render::modals::RenameProjectModal(createdProj);
        WindowStack::addWindowToStack(_renameProjects[createdProj]);
        _deleteProjects[createdProj] = new editor::render::modals::DeleteProjectModal(createdProj);
        WindowStack::addWindowToStack(_deleteProjects[createdProj]);
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

std::string editor::render::windows::WelcomeWindow::searchProject() {
    const char* fileExtension[] = {"*.lua"};
    const char* filePath = tinyfd_openFileDialog(
            io::LocalizationManager::GetInstance().getString("action.selectprojectfile").c_str(),
            "",
            1,
            fileExtension,
            io::LocalizationManager::GetInstance().getString("file.projectfile").c_str(),
            0);
    if(filePath == nullptr) return "";
    else return std::string(filePath);
}

