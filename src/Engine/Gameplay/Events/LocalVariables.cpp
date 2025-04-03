#include "LocalVariables.h"
#include <Core/ComponentData.h>
#include <Utils/Error.h>

LocalVariables::LocalVariables(ComponentData const* data) :
    ComponentTemplate(data) {
}

bool LocalVariables::init() {
    auto& data = _data->getData();
    for (auto& [key, value] : data) {
        if (!key.is<std::string>()) {
            Error::ShowError("LocalVariables", "key is not a string");
            return false;
        }
        variables.insert({key.as<std::string>(), value.as<sol::lua_value>()});
    }
    return true;
}

sol::lua_value const& LocalVariables::getVariable(std::string const& name) const {
    auto it = variables.find(name);
    if (it == variables.end())
        return sol::lua_nil;
    return it->second;
}

bool LocalVariables::setVariable(std::string const& name, sol::lua_value const& value) {
    auto it = variables.find(name);
    if (it == variables.end())
        return false;
    it->second = value;
    return true;
}

void LocalVariables::RegisterToLua(sol::state& luaState) {
    sol::usertype<LocalVariables> type = luaState.new_usertype<LocalVariables>("LocalVariables");
    type["getVariable"] = &LocalVariables::getVariable;
    type["setVariable"] = &LocalVariables::setVariable;
    type["get"] = LocalVariables::get;
}


