#ifndef COMPONENTDATA_H
#define COMPONENTDATA_H

#include <sol/sol.hpp>

class ComponentData {
public:
    ComponentData(std::string const& id, sol::table const& data);
};



#endif //COMPONENTDATA_H
