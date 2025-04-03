#ifndef SHOWCHOICES_H
#define SHOWCHOICES_H
#include <string>
#include <Core/ComponentTemplate.h>
#include <sol/lua_value.hpp>


class ComponentClass(ShowChoices) {
    private:
        std::string _variable;
        std::vector<sol::lua_value> _options;
    public:
        ShowChoices(ComponentData const* data);
        bool init() override;
        bool update() override;
        bool ended() const;
};



#endif //SHOWCHOICES_H
