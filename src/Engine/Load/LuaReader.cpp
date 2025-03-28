#include "LuaReader.h"

LuaReader* LuaReader::_instance = nullptr;

bool LuaReader::init() {
    _lua.open_libraries(sol::lib::base, sol::lib::package);
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
