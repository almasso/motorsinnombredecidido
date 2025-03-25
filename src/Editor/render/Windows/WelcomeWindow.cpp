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
#include <iostream>

editor::render::windows::WelcomeWindow::WelcomeWindow() : Window("welcomeWindow") {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
          ImGuiWindowFlags_NoCollapse |
          ImGuiWindowFlags_NoScrollbar |
          ImGuiWindowFlags_NoScrollWithMouse |
          ImGuiWindowFlags_NoDocking;
    _windowFlags |= flags;
    RenderManager::GetInstance().setWindowName(io::LocalizationManager::GetInstance().getString("window.welcomewindow.title") + " " + io::LocalizationManager::GetInstance().getString("window.welcomewindow.currentversion"));

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
        ImGui::Text(io::LocalizationManager::GetInstance().getString("window.welcomewindow.title").c_str());
        ImGui::PopFont();
        ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 40"));
        ImGui::Text(io::LocalizationManager::GetInstance().getString("window.welcomewindow.currentversion").c_str());
        ImGui::PopFont();

        ImGui::SetCursorPosY(windowHeight - 100);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 1)); // Fondo del botón
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1)); // Al pasar el ratón por encima
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1)); // Al pulsar el botón
        ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
        if (ImGui::Button(ICON_FA_GEAR, ImVec2(100, 100))) {
            // Llamada a los ajustes del editor
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
                if (filter.PassFilter(pr->getName().c_str()) || filter.PassFilter(pr->getPath().string().c_str())) {
                    if (pr != *io::ProjectManager::GetInstance().getProjects().begin()) {
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                    drawProjectButton(pr->getName(), pr->getPath().string(), pr->getLastModificationTime());
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
            newProjectModal();
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
            searchProject();
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

void editor::render::windows::WelcomeWindow::drawProjectButton(const std::string& projectName, const std::string& projectRoute, const std::tm& lastModified) {
    ImVec2 buttonSize = ImVec2(-1, 120);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.1, 0.1, 1)); // Fondo del botón
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1)); // Al pasar el ratón por encima
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1)); // Al pulsar el botón

    ImGui::Button(std::string("##But" + projectRoute).c_str() , buttonSize);

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
}

void editor::render::windows::WelcomeWindow::newProjectModal() {

}

void editor::render::windows::WelcomeWindow::searchProject() {
    const char* fileExtension[] = {"*.lua"};
    const char* filePath = tinyfd_openFileDialog(
            io::LocalizationManager::GetInstance().getString("action.selectprojectfile").c_str(),
            "",
            1,
            fileExtension,
            io::LocalizationManager::GetInstance().getString("file.projectfile").c_str(),
            0);
    if(filePath == nullptr) return;
    io::ProjectManager::GetInstance().addProject(filePath);
}