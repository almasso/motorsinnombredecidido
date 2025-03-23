#include "LuaReader.h"

LuaReader* LuaReader::_instance = nullptr;

bool LuaReader::init() {
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
    sol::load_result res = _instance->_lua.load_file(path);
    if (!res.valid())
        return nullptr;
    sol::protected_function_result res2 = res();
    if (!res2.valid())
        return nullptr;
    sol::table table = res2.get<sol::table>();
    if (!table.valid())
        return nullptr;
    return table;
}

sol::table LuaReader::GetTable(sol::table const& container, std::string const& name) {
    return container.get_or<sol::table>(name, nullptr);
}

sol::function LuaReader::GetFunction(std::string const& path) {
    sol::load_result res = _instance->_lua.load_file(path);
    if (!res.valid())
        return nullptr;
    sol::protected_function_result res2 = res();
    if (!res2.valid())
        return nullptr;
    sol::function function = res2.get<sol::function>();
    if (!function.valid())
        return nullptr;
    return function;
}

sol::function LuaReader::GetFunction(sol::table const& container, std::string const& name) {
    return container.get_or<sol::function>(name, nullptr);
}
