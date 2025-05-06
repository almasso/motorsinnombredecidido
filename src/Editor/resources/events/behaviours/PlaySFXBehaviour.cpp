//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "PlaySFXBehaviour.h"

#include <imgui.h>
#include <common/EditorError.h>
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <resources/events/Event.h>
#include <utils/tinyfiledialogs/tinyfiledialogs.h>

#define sfxKey "sfx"

#define MAX_CLIP_BUFFER 1024

editor::resources::events::PlaySFXBehaviour::PlaySFXBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _sfxSource(new char[MAX_CLIP_BUFFER]) {
    _sfxSource[0] = '\0';
}

editor::resources::events::PlaySFXBehaviour::~PlaySFXBehaviour() {
    delete[] _sfxSource;
}

bool editor::resources::events::PlaySFXBehaviour::read(sol::table const& params) {
    sol::optional<std::string> sfxHandler = params.get<sol::optional<std::string>>(sfxKey);
    if (!sfxHandler.has_value())
        return false;
    sfxHandler.value().copy(_sfxSource, MAX_CLIP_BUFFER - 1);
    _sfxSource[MAX_CLIP_BUFFER - 1] = '\0';
    return true;
}

bool editor::resources::events::PlaySFXBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) {
    std::string handler(std::to_string(reinterpret_cast<long long>(this)) + std::to_string(reinterpret_cast<long long>(container)));
    behaviour << "\"" << handler << "\"" ;
    writeDependencies(dependencies, handler);
    return true;
}

bool editor::resources::events::PlaySFXBehaviour::render() {
    return renderAudioSelector();
}

bool editor::resources::events::PlaySFXBehaviour::writeParams(sol::table& params) {
    params[sfxKey] = _sfxSource;
    return true;
}

bool editor::resources::events::PlaySFXBehaviour::renderAudioSelector() {
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
                strncpy(_sfxSource, "", MAX_CLIP_BUFFER - 1);
                _sfxSource[MAX_CLIP_BUFFER - 1] = '\0';
            }
            else {
                std::string fR(std::filesystem::path(route).lexically_normal().string());
                strncpy(_sfxSource, fR.c_str(), MAX_CLIP_BUFFER - 1);
                _sfxSource[MAX_CLIP_BUFFER - 1] = '\0';
                edited = true;
            }
        }
    }
    ImGui::SameLine();
    edited = ImGui::InputText((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.PlaySFXBehaviour.source") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(),
                     _sfxSource, MAX_CLIP_BUFFER, ImGuiInputTextFlags_EnterReturnsTrue) || edited;
    return edited;
}

void editor::resources::events::PlaySFXBehaviour::writeDependencies(EventBuildDependencies& dependencies, std::string const& handler) {
    EventBuildDependencies::ComponentsMap components;
    auto& lua = io::LuaManager::GetInstance().getState();

    sol::table audioSourceParams = lua.create_table();
    std::filesystem::path source(_sfxSource);
    source = ("data" / source.lexically_relative(_event->getProject()->getPath()));
    audioSourceParams["clip"] = source.string();
    audioSourceParams["mixer"] = "data/mixers/sfx.mixer.lua";
    components.insert({"AudioSource", audioSourceParams});

    dependencies.childrenDependencies.push_back({handler, components});
}
