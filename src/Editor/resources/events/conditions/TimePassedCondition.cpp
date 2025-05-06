//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "TimePassedCondition.h"

#include <imgui.h>
#include <io/LocalizationManager.h>

#define secondsKey "seconds"

editor::resources::events::TimePassedCondition::TimePassedCondition(Event* event) :
    EventConditionTemplate(event),
    _seconds(0.0f) {
}

editor::resources::events::TimePassedCondition::~TimePassedCondition() = default;

bool editor::resources::events::TimePassedCondition::read(sol::table const& params) {
    _seconds = params.get_or<float, std::string, float>(secondsKey, -1.0f);
    return _seconds >= 0.0f;
}

bool editor::resources::events::TimePassedCondition::render() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition.TimePassed.seconds").c_str());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::InputFloat(("##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), &_seconds);
}

bool editor::resources::events::TimePassedCondition::writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) {
    condition << secondsKey << " = " << _seconds << "\n";
    return true;
}

bool editor::resources::events::TimePassedCondition::writeParams(sol::table& params) {
    params[secondsKey] = _seconds;
    return true;
}
