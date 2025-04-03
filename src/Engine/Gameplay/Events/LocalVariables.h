#ifndef LOCALVARIABLES_H
#define LOCALVARIABLES_H

#include <unordered_map>
#include <Core/ComponentTemplate.h>
#include <sol/lua_value.hpp>

class ComponentClass(LocalVariables) {
private:
    std::unordered_map<std::string, sol::lua_value> variables;
public:
    explicit LocalVariables(ComponentData const* data);
    bool init() override;

    sol::lua_value const& getVariable(std::string const& name) const;
    bool setVariable(std::string const& name, sol::lua_value const& value);

    static void RegisterToLua(sol::state& luaState);
};



#endif //LOCALVARIABLES_H
