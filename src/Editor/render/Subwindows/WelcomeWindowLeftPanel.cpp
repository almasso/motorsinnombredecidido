//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WelcomeWindowLeftPanel.h"
#include "render/RenderManager.h"
#include "io/LocalizationManager.h"
#include "utils/IconsFontAwesome6.h"
#include "render/Modals/SettingsModal.h"

editor::render::subwindows::WelcomeWindowLeftPanel::WelcomeWindowLeftPanel(editor::render::modals::SettingsModal* settings) :
Subwindow("WWLP"), _settings(settings) {}

void editor::render::subwindows::WelcomeWindowLeftPanel::beforeRender() {
    float leftPanelWidth = RenderManager::GetInstance().getWidth() / 2 - 90;
    float windowHeight = ImGui::GetContentRegionAvail().y;
    _size = ImVec2(leftPanelWidth, windowHeight);
}

void editor::render::subwindows::WelcomeWindowLeftPanel::onRender() {
    float windowHeight = ImGui::GetContentRegionAvail().y;

    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 90"));
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.global.editorname").c_str());
    ImGui::PopFont();
    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 40"));
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.global.currentversion").c_str());
    ImGui::PopFont();

    ImGui::SetCursorPosY(windowHeight - 100);
    ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
    if (ImGui::Button(ICON_FA_GEAR, ImVec2(100, 100))) {
        _settings->show();
    }
    ImGui::PopFont();

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
