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
#include <Render/Animator.h>
#include <Render/Transform.h>

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

    Entity::RegisterToLua(_lua);
    Scene::RegisterToLua(_lua);
    Game::RegisterToLua(_lua);
}

bool LuaReader::init() {
    _lua.open_libraries(sol::lib::base, sol::lib::package);
    registerUserTypes();
    return true;
}

LuaReader::LuaReader() = default;

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
    sol::load_result res = _instance->_lua.load_file(path);
    if (!res.valid())
        return sol::lua_nil;
    sol::protected_function_result res2 = res();
    if (!res2.valid())
        return sol::lua_nil;
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
