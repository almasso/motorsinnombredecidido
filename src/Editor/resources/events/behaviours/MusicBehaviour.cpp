//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MusicBehaviour.h"

#include <imgui.h>
#include <common/EditorError.h>
#include <io/LocalizationManager.h>
#include <resources/events/Event.h>
#include <utils/tinyfiledialogs/tinyfiledialogs.h>
#include <common/Project.h>
#include <render/RenderManager.h>

#define actionKey "action"
#define clipKey "clip"
#define volumeKey "volume"
#define loopKey "loop"

#define MAX_CLIP_BUFFER 1024

editor::resources::events::MusicBehaviour::MusicBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _action(PLAY),
    _param() {
}

editor::resources::events::MusicBehaviour::~MusicBehaviour() {
    if (_action == CHANGE)
        delete[] _param.clip;
}

bool editor::resources::events::MusicBehaviour::read(sol::table const& params) {
    std::string action = params.get_or<std::string>(actionKey, "");
    if (action == "play")
        _action = PLAY;
    else if (action == "stop")
        _action = STOP;
    else if (action == "resume")
        _action = RESUME;
    else if (action == "pause")
        _action = PAUSE;
    else if (action == "change") {
        _action = CHANGE;
        sol::optional<std::string> clip = params.get<sol::optional<std::string>>(clipKey);
        if (!clip.has_value())
            return false;
        _param.clip = new char[MAX_CLIP_BUFFER];
        std::string clipPath = _event->getProject()->getAssetsPath().string() + "/" + clip.value();
        clipPath.copy(_param.clip, MAX_CLIP_BUFFER - 1);
        _param.clip[clipPath.size()] = '\0';
    }
    else if (action == "volume") {
        _action = VOLUME;
        sol::optional<float> volume = params.get<sol::optional<float>>(volumeKey);
        if (!volume.has_value())
            return false;
        _param.volume = volume.value();
    }
    else if (action == "loop") {
        _action = LOOP;
        sol::optional<bool> loop = params.get<sol::optional<bool>>(loopKey);
        if (!loop.has_value())
            return false;
        _param.loop = loop.value();
    }
    else return false;
    return true;
}

bool editor::resources::events::MusicBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) {
    sol::table actionParams = io::LuaManager::GetInstance().getState().create_table();
    switch (_action) {
    case PLAY:
        actionParams[actionKey] = "play";
        break;
    case STOP:
        actionParams[actionKey] = "stop";
        break;
    case RESUME:
        actionParams[actionKey] = "resume";
        break;
    case PAUSE:
        actionParams[actionKey] = "pause";
        break;
    case CHANGE: {
            actionParams[actionKey] = "change";
            std::filesystem::path clip(_param.clip);
            clip = "data" / clip.lexically_relative(_event->getProject()->getPath());
            std::string clipPath = clip.string();
            std::ranges::replace(clipPath, '\\', '/');
            actionParams[clipKey] = clipPath;
        }
        break;
    case VOLUME:
        actionParams[actionKey] = "volume";
        actionParams[volumeKey] = _param.volume;
        break;
    case LOOP:
        actionParams[actionKey] = "loop";
        actionParams[loopKey] = _param.loop;
        break;
    }
    behaviour << io::LuaManager::GetInstance().serializeToString(actionParams);
    return true;
}

bool editor::resources::events::MusicBehaviour::render() {
    bool edited = renderActionSelector();
    switch (_action) {
    case CHANGE:
        edited = renderChangeAction() || edited;
        break;
    case VOLUME:
        edited = renderVolumeAction() || edited;
        break;
    case LOOP:
        edited = renderLoopAction() || edited;
        break;
    default: ;
    }
    return edited;
}

bool editor::resources::events::MusicBehaviour::writeParams(sol::table& params) {
    switch (_action) {
    case PLAY:
        params[actionKey] = "play";
        break;
    case STOP:
        params[actionKey] = "stop";
        break;
    case RESUME:
        params[actionKey] = "resume";
        break;
    case PAUSE:
        params[actionKey] = "pause";
        break;
    case CHANGE: {
            params[actionKey] = "change";
            std::filesystem::path source(_param.clip);
            source = source.lexically_relative(_event->getProject()->getAssetsPath());
            std::string sourcePath = source.string();
            std::ranges::replace(sourcePath, '\\', '/');
            params[clipKey] = sourcePath;
            break;
        }
    case VOLUME:
        params[actionKey] = "volume";
        params[volumeKey] = _param.volume;
        break;
    case LOOP:
        params[actionKey] = "loop";
        params[loopKey] = _param.loop;
        break;
    }
    return true;
}

bool editor::resources::events::MusicBehaviour::renderActionSelector() {
    bool edited = false;
    std::string actionName = getActionName(_action);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("###musicActionSelector") + std::to_string(reinterpret_cast<long long>(this))).c_str(), actionName.c_str()))
        return false;

    for (int i = 0; i < MAX_ACTION; ++i) {
        bool isSelected = (i == _action);
        if (ImGui::Selectable((getActionName(static_cast<MUSIC_ACTION>(i)) + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), isSelected)) {
            if (!isSelected) {
                if (_action == CHANGE)
                    delete[] _param.clip;
                _action = static_cast<MUSIC_ACTION>(i);
                if (_action == CHANGE) {
                    _param.clip = new char[MAX_CLIP_BUFFER];
                    _param.clip[0] = '\0';
                }
                else if (_action == VOLUME) {
                    _param.volume = 1.0f;
                }
                else if (_action == LOOP) {
                    _param.loop = true;
                }
                edited = true;
            }
        }
    }
    ImGui::EndCombo();
    return edited;
}

bool editor::resources::events::MusicBehaviour::renderChangeAction() {
    bool edited = false;
    auto project = _event->getProject();
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("action.search").c_str())) {
        const char* fileExtension[] = {"*.wav"};
        const char* route = tinyfd_openFileDialog(
                io::LocalizationManager::GetInstance().getString("action.selectaudio").c_str(),
                (project->getPath() / "assets/").string().c_str(),
                1,
                fileExtension,
                nullptr,
                0
        );
        if(route != nullptr) {
            if(std::filesystem::path(route).parent_path() != (project->getPath() / "assets")) {
                showUserWarning(io::LocalizationManager::GetInstance().getString("error.assetlocationnotvalid"))
                strncpy(_param.clip, "", MAX_CLIP_BUFFER - 1);
                _param.clip[MAX_CLIP_BUFFER - 1] = '\0';
            }
            else {
                std::string fR(std::filesystem::path(route).lexically_normal().string());
                strncpy(_param.clip, fR.c_str(), MAX_CLIP_BUFFER - 1);
                _param.clip[MAX_CLIP_BUFFER - 1] = '\0';
                edited = true;
            }
        }
    }
    ImGui::SameLine();
    edited = ImGui::InputText((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.change.source") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(),
                     _param.clip, MAX_CLIP_BUFFER - 1, ImGuiInputTextFlags_EnterReturnsTrue) || edited;
    _param.clip[MAX_CLIP_BUFFER - 1] = '\0';
    return edited;
}

bool editor::resources::events::MusicBehaviour::renderVolumeAction() {
    return ImGui::SliderFloat((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.volume.volume") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), &_param.volume, 0, 1);
}

bool editor::resources::events::MusicBehaviour::renderLoopAction() {
    return ImGui::Checkbox((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.loop.loop") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(), &_param.loop);
}

std::string editor::resources::events::MusicBehaviour::getActionName(MUSIC_ACTION action) const {
    switch (action) {
    case PLAY:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.play");
    case STOP:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.stop");
    case RESUME:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.resume");
    case PAUSE:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.pause");
    case CHANGE:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.change");
    case VOLUME:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.volume");
    case LOOP:
        return io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.MusicBehaviour.action.loop");
    default:
        return "";
    }
}

