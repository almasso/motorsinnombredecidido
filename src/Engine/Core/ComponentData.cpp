#include "ComponentData.h"

ComponentData::ComponentData(std::string const& id, sol::table const& data) : _id(id), _data(data) {
}

const std::string& ComponentData::getId() const {
    return _id;
}

Vector2 ComponentData::getVector(std::string const &key, Vector2 const &defaultValue) const {
    sol::optional<sol::table> vector = _data[key];
    if (!vector || vector->size() != 2) {
        return defaultValue;
    }
    sol::optional<float> x = (*vector)[1];
    sol::optional<float> y = (*vector)[2];
    if (!x || !y) {
        return defaultValue;
    }
    return Vector2(*x, *y);
}

std::unordered_set<std::string> ComponentData::getSet(std::string const &key) const {
    std::unordered_set<std::string> set;
    if (sol::optional<sol::table> table = _data[key]; table) {
        for (auto&[first, second] : *table) {
            set.insert(second.as<std::string>());
        }
    }
    return set;
}
