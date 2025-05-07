//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "AnimationBehaviour.h"

#include <imgui.h>
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <resources/events/Event.h>

#define actionKey "action"
#define animationChangeKey "animation"

editor::resources::events::AnimationBehaviour::AnimationBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _action(PLAY),
    _animationToChange("") {
}

editor::resources::events::AnimationBehaviour::~AnimationBehaviour() = default;

bool editor::resources::events::AnimationBehaviour::read(sol::table const& params) {
    std::string action = params.get_or<std::string>(actionKey, "");
    if (action.empty())
        return false;
    if (action == "play")
        _action = PLAY;
    else if (action == "stop")
        _action = STOP;
    else if (action == "reset")
        _action = RESET;
    else if (action == "change") {
        _action = CHANGE;
        _animationToChange = params.get_or<std::string>(animationChangeKey, "");
        if (_animationToChange.empty())
            return false;
    }
    else return false;

    return true;
}

bool editor::resources::events::AnimationBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) {
    auto& luaManager = io::LuaManager::GetInstance();

    dependencies.componentDependencies.insert({"Animator", {}});

    sol::table actionParams = luaManager.getState().create_table();
    switch (_action) {
    case PLAY:
        actionParams[actionKey] = "play";
        break;
    case STOP:
        actionParams[actionKey] = "stop";
        break;
    case RESET:
        actionParams[actionKey] = "reset";
        break;
    case CHANGE:
        actionParams[actionKey] = "change";
        break;
    }
    if (_action == CHANGE) {
        std::filesystem::path animPath = std::filesystem::path("data") / "animations" / (_animationToChange + ".lua");
        actionParams[animationChangeKey] = animPath.string();
    }
    std::string serializedParams = luaManager.serializeToString(actionParams);
    if (serializedParams.empty())
        return false;
    behaviour << serializedParams;
    return true;
}

bool editor::resources::events::AnimationBehaviour::render() {
    bool edited = renderActionSelector();
    if (_action == CHANGE)
        edited = renderAnimationSelector() || edited;
    return edited;
}

bool editor::resources::events::AnimationBehaviour::writeParams(sol::table& params) {
    switch (_action) {
    case PLAY:
        params[actionKey] = "play";
        break;
    case STOP:
        params[actionKey] = "stop";
        break;
    case RESET:
        params[actionKey] = "reset";
        break;
    case CHANGE:
        params[actionKey] = "change";
        params[animationChangeKey] = _animationToChange;
        break;
    }
    return true;
}

bool editor::resources::events::AnimationBehaviour::renderActionSelector() {
    bool edited = false;
    std::string actionName = getActionName(_action);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("###animationActionSelector") + std::to_string(reinterpret_cast<long long>(this))).c_str(), actionName.c_str()))
        return false;

    for (int i = 0; i < MAX_ACTION; ++i) {
        bool isSelected = (i == _action);
        if (ImGui::Selectable((getActionName(static_cast<ANIMATION_ACTION>(i)) + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), isSelected)) {
            if (!isSelected) {
                _action = static_cast<ANIMATION_ACTION>(i);
                edited = true;
            }
        }
    }
    ImGui::EndCombo();
    return edited;
}

bool editor::resources::events::AnimationBehaviour::renderAnimationSelector() {
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    auto const& animations = _event->getProject()->getAnimations();
    std::string preview;
    if (animations.contains(_animationToChange))
        preview = _animationToChange;
    else preview = io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.AnimationBehaviour.action.change.select");

    if (!ImGui::BeginCombo("##animationChangeSelector", preview.c_str()))
        return false;

    bool edited = false;
    for (auto& [animName, anim] : animations) {
        bool isSelected = _animationToChange == animName;
        if(ImGui::Selectable(animName.c_str(), isSelected)) {
            _animationToChange = animName;
            edited = true;
        }
    }

    ImGui::EndCombo();
    return edited;
}

std::string editor::resources::events::AnimationBehaviour::getActionName(ANIMATION_ACTION action) const {
    switch (action) {
    case PLAY:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.AnimationBehaviour.action.play");
    case STOP:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.AnimationBehaviour.action.stop");
    case RESET:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.AnimationBehaviour.action.reset");
    case CHANGE:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.AnimationBehaviour.action.change");
    default:
        return "";
    }
}
