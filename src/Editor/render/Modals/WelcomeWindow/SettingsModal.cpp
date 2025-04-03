//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "SettingsModal.h"
#include "imgui.h"
#include "io/LocalizationManager.h"

editor::render::modals::SettingsModal::SettingsModal() : ModalWindow(
        io::LocalizationManager::GetInstance().getString("action.settings").c_str()) {
    std::unordered_set<std::string> languages = io::LocalizationManager::GetInstance().getAllLanguages();
    for(const auto& lang : languages) {
        _languagesID.push_back(lang);
        _languages.push_back(io::LocalizationManager::GetInstance().getString(lang).c_str());
    }

    int i = 0;
    std::string currentLanguage = io::LocalizationManager::GetInstance().getCurrentLanguage();
    while(currentLanguage != _languagesID[i]) ++i;
    _languagesSelection = i;
}

void editor::render::modals::SettingsModal::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::SettingsModal::onRender() {
    ImGui::Combo(io::LocalizationManager::GetInstance().getString("language.language").c_str(), &_languagesSelection, _languages.data(), _languages.size());

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.save").c_str(), ImVec2(120, 0))) {
        io::LocalizationManager::GetInstance().changePreferredLocale(_languagesID[_languagesSelection]);
        _objectName = io::LocalizationManager::GetInstance().getString("action.settings").c_str();
        reset();
        ImGui::CloseCurrentPopup();
        _isOpen = false;
    }
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
    }
}

void editor::render::modals::SettingsModal::reset() {
    _languages.clear();
    std::unordered_set<std::string> languages = io::LocalizationManager::GetInstance().getAllLanguages();
    for(const auto& lang : languages) {
        _languages.push_back(io::LocalizationManager::GetInstance().getString(lang).c_str());
    }
}