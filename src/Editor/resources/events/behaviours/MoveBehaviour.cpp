//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MoveBehaviour.h"

#include <imgui.h>
#include <io/LocalizationManager.h>

#define xTargetKey "xTarget"
#define yTargetKey "yTarget"

editor::resources::events::MoveBehaviour::MoveBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _xTarget(0),
    _yTarget(0) {
}

editor::resources::events::MoveBehaviour::~MoveBehaviour() = default;

bool editor::resources::events::MoveBehaviour::read(sol::table const& params) {
    sol::optional<int> xTarget = params.get<sol::optional<int>>(xTargetKey);
    if (!xTarget.has_value())
        return false;
    _xTarget = xTarget.value();

    sol::optional<int> yTarget = params.get<sol::optional<int>>(yTargetKey);
    if (!yTarget.has_value())
        return false;
    _yTarget = yTarget.value();

    return true;
}

bool editor::resources::events::MoveBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::MoveBehaviour::render() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours." + std::string(id) + ".target").c_str());
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours." + std::string(id) + ".x").c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    bool edited = ImGui::InputInt(std::string("##moveTargetX" + std::to_string(reinterpret_cast<long long>(this))).c_str(), &_xTarget);
    ImGui::SameLine();
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours." + std::string(id) + ".y").c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    edited |= ImGui::InputInt(std::string("##moveTargetY" + std::to_string(reinterpret_cast<long long>(this))).c_str(), &_yTarget);
    return edited;
}

bool editor::resources::events::MoveBehaviour::writeParams(sol::table& params) {
    params[xTargetKey] = _xTarget;
    params[yTargetKey] = _yTarget;
    return true;
}
