#include "LuaReader.h"

#include <Audio/AudioSource.h>
#include <Collisions/Collider.h>
#include <Core/Entity.h>
#include <Core/Game.h>
#include <Core/Scene.h>
#include <Gameplay/Dialog/TextBox.h>
#include <Gameplay/Events/LocalVariables.h>
#include <Gameplay/Events/Event.h>
#include <Gameplay/Events/EventBehaviour.h>
#include <Gameplay/Events/EventCondition.h>
#include <Gameplay/Events/EventConditionFactory.h>
#include <Gameplay/Events/EventHandler.h>
#include <Gameplay/Movement/MovementComponent.h>
#include <Input/Button.h>
#include <Render/Animator.h>
#include <Render/Text.h>
#include <Render/Transform.h>
#include <Utils/Error.h>

LuaReader* LuaReader::_instance = nullptr;

void LuaReader::registerUserTypes() {
    Vector2::RegisterToLua(_lua);

    EventBehaviour::RegisterToLua(_lua);
    EventCondition::RegisterToLua(_lua);
    EventConditionFactory::RegisterToLua(_lua);
    Event::RegisterToLua(_lua);
    EventHandler::RegisterToLua(_lua);

    Transform::RegisterToLua(_lua);
    AudioSource::RegisterToLua(_lua);
    Collider::RegisterToLua(_lua);
    Animator::RegisterToLua(_lua);
    MovementComponent::RegisterToLua(_lua);
    LocalVariables::RegisterToLua(_lua);
    TextBox::RegisterToLua(_lua);
    Text::RegisterToLua(_lua);
    Button::RegisterToLua(_lua);

    Entity::RegisterToLua(_lua);
    Scene::RegisterToLua(_lua);
    Game::RegisterToLua(_lua);
}

bool LuaReader::init() {
    _lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
    _lua.add_package_loader([&](std::string const& module){
        std::string filename = module;
        std::ranges::replace(filename, '.', '/');
        filename += ".lua";
        std::string fileContent;
        if (!ReadFile(filename, fileContent))
            return sol::make_object(_lua, ("\n\tno bindings module " + module));
        return _instance->_lua.load(fileContent).get<sol::object>();
    });
    registerUserTypes();
    return true;
}

LuaReader::LuaReader() = default;

bool LuaReader::ReadFile(const std::string& filename, std::string& fileContent) {
    SDL_IOStream* file = SDL_IOFromFile(filename.c_str(), "r");
    if (!file) {
        Error::ShowError("Error al abrir el archivo", "Error al abrir el archivo: " + filename + " - " + SDL_GetError());
        return false;
    }

    int fileSize = static_cast<int>(SDL_GetIOSize(file));
    if (fileSize <= 0) {
        Error::ShowError("Error al obtener el tamaño del archivo", "Error al obtener el tamaño del archivo: " + filename + " - " + SDL_GetError());
        SDL_CloseIO(file);
        return false;
    }

    char* fileData = new char[fileSize + 1];  // +1 para el null terminator
    if (SDL_ReadIO(file, fileData, fileSize) != fileSize) {
        Error::ShowError("Error al leer el archivo", "Error al leer el archivo: " + filename + " - " + SDL_GetError());
        delete[] fileData;
        SDL_CloseIO(file);
        return false;
    }
    fileData[fileSize] = '\0';

    fileContent = fileData;
    delete[] fileData;

    SDL_CloseIO(file);
    return true;
}

bool LuaReader::Init() {
    assert(_instance == nullptr);
    _instance = new LuaReader();
    if (_instance->init())
        return true;
    Shutdown();
    return false;
}

void LuaReader::Shutdown() {
    delete _instance;
}

sol::table LuaReader::GetTable(std::string const& path) {
    std::string fileContent;
    if (!ReadFile(path, fileContent))
        return sol::lua_nil;
    sol::load_result res = _instance->_lua.load(fileContent);
    if (!res.valid())
        return sol::lua_nil;
    sol::protected_function_result res2 = res();
    if (!res2.valid()) {
        sol::error er = res2;
        Error::ShowError("GetTable", er.what());
        return sol::lua_nil;
    }
    sol::table table = res2.get<sol::table>();
    if (!table.valid())
        return sol::lua_nil;
    return table;
}

sol::table LuaReader::GetTable(sol::table const& container, std::string const& name) {
    sol::optional<sol::table> table = container[name];
    if (!table)
        return sol::lua_nil;
    return *table;
}

sol::function LuaReader::GetFunction(sol::table const& container, std::string const& name) {
    sol::optional<sol::function> function = container[name];
    if (!function)
        return sol::lua_nil;
    return *function;
}
