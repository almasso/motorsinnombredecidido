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
    _sfxHandler(new char[MAX_CLIP_BUFFER]) {
    _sfxHandler[0] = '\0';
}

editor::resources::events::PlaySFXBehaviour::~PlaySFXBehaviour() {
    delete[] _sfxHandler;
}

bool editor::resources::events::PlaySFXBehaviour::read(sol::table const& params) {
    sol::optional<std::string> sfxHandler = params.get<sol::optional<std::string>>(sfxKey);
    if (!sfxHandler.has_value())
        return false;
    sfxHandler.value().copy(_sfxHandler, MAX_CLIP_BUFFER);
    return true;
}

bool editor::resources::events::PlaySFXBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::PlaySFXBehaviour::render() {
    return renderAudioSelector();
}

bool editor::resources::events::PlaySFXBehaviour::writeParams(sol::table& params) {
    params[sfxKey] = _sfxHandler;
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
                strncpy(_sfxHandler, "", MAX_CLIP_BUFFER - 1);
                _sfxHandler[MAX_CLIP_BUFFER - 1] = '\0';
            }
            else {
                std::string fR(std::filesystem::path(route).lexically_normal().string());
                strncpy(_sfxHandler, fR.c_str(), MAX_CLIP_BUFFER - 1);
                _sfxHandler[MAX_CLIP_BUFFER - 1] = '\0';
                edited = true;
            }
        }
    }
    ImGui::SameLine();
    edited = ImGui::InputText((io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.PlaySFXBehaviour.source") + "##" + std::to_string(reinterpret_cast<long long>(this))).c_str(),
                     _sfxHandler, IM_ARRAYSIZE(_sfxHandler), ImGuiInputTextFlags_EnterReturnsTrue) || edited;
    return edited;
}
