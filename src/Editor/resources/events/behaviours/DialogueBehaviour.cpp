//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "DialogueBehaviour.h"

#include <imgui.h>

#define textKey "text"
#define MAX_DIALOGUE 256

editor::resources::events::DialogueBehaviour::DialogueBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _text(new char[MAX_DIALOGUE]) {
    _text[0] = '\0';
}

editor::resources::events::DialogueBehaviour::~DialogueBehaviour() {
    delete[] _text;
}

bool editor::resources::events::DialogueBehaviour::read(sol::table const& params) {
    sol::optional<std::string> text = params.get<sol::optional<std::string>>(textKey);
    if (!text.has_value())
        return false;
    text.value().copy(_text, MAX_DIALOGUE);
    _text[text.value().size()] = '\0';
    return true;
}

bool editor::resources::events::DialogueBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) {
    behaviour << "\"" << _text << "\"";
    return true;
}

bool editor::resources::events::DialogueBehaviour::render() {
    bool edited =  ImGui::InputTextMultiline(std::string("##dialogueInput" + std::to_string(reinterpret_cast<long long>(this))).c_str(),
        _text, MAX_DIALOGUE);
    return edited;
}

bool editor::resources::events::DialogueBehaviour::writeParams(sol::table& params) {
    params[textKey] = _text;
    return true;
}
