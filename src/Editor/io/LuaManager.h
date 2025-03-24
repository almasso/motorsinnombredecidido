//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_LUAMANAGER_H
#define MOTORSINNOMBREDECIDIDO_LUAMANAGER_H

#include <memory>
#include <string>
#include <sol/sol.hpp>

namespace editor::io {
    class LuaManager {
    public:
        static bool Init();

        static LuaManager& GetInstance();

        template <std::convertible_to<std::string> T>
        sol::table getTable(T&& filename) {
            return _getTable(std::forward<T>(filename));
        }

        [[nodiscard]] sol::state& getState() const;

        template <std::convertible_to<std::string> T>
        sol::table getTableFromScript(T&& filename) {
            return _getTableFromScript(std::forward<T>(filename));
        }

        std::string serializeTable(const sol::table& table);

        LuaManager(const LuaManager &) = delete;

        LuaManager &operator=(const LuaManager &) = delete;

        ~LuaManager();
    private:
        static std::unique_ptr<LuaManager> _instance;

        std::unique_ptr<sol::state> _state;

        LuaManager() = default;

        bool init();

        sol::table _getTable(const std::string& filename);

        sol::table _getTableFromScript(const std::string& filename);
    };
}


#endif //MOTORSINNOMBREDECIDIDO_LUAMANAGER_H
