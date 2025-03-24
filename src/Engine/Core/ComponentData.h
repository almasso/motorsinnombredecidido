#ifndef COMPONENTDATA_H
#define COMPONENTDATA_H

#include <sol/sol.hpp>
#include <Utils/Vector2.h>

class ComponentData {
private:
    std::string _id;
    sol::table _data;
public:
    ComponentData(std::string const& id, sol::table const& data);
    template<typename T>
    T getData(std::string const& key, T const& defaultValue = {}) const {
        return _data.get_or(key, defaultValue);
    }
    const std::string& getId() const;
};


#endif //COMPONENTDATA_H
