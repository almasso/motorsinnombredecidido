#ifndef COMPONENTDATA_H
#define COMPONENTDATA_H
#include <Utils/Vector2.h>
#include <sol/sol.hpp>
#include <string>
#include <array>
#include <unordered_set>

class ComponentData {
private:
    std::string _id;
    sol::table _data;
public:
    ComponentData(std::string const& id, sol::table const& data);
    Vector2 getVector(std::string const& key, Vector2 const& defaultValue = {}) const;
    std::unordered_set<std::string> getSet(std::string const& key) const;
    template<typename T>
    T getData(std::string const& key, T const& defaultValue = {}) const {
        return _data.get_or(key, defaultValue);
    }
    template<typename T, size_t N>
    std::array<T,N> getArray(std::string const& key, T const& defaultValue = {}) const {
        std::array<T, N> result;
        sol::optional<sol::table> table = _data[key];

        if (!table) {
            result.fill(defaultValue);
            return result;
        }

        sol::table tbl = table.value();
        for (size_t i = 0; i < N; ++i) {
            sol::optional<T> val = tbl[i + 1];
            result[i] = val.value_or(defaultValue);
        }

        return result;
    }
    const std::string& getId() const;
    sol::table const& getData() const;
};


#endif //COMPONENTDATA_H
