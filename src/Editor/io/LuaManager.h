//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_LUAMANAGER_H
#define MOTORSINNOMBREDECIDIDO_LUAMANAGER_H

#include <memory>
#include <string>

namespace sol {
    class state;
}

namespace editor::io {
    class LuaManager {
    public:
        static bool Init();

        static LuaManager& GetInstance();

        template <std::convertible_to<std::string> T>
        bool loadFile(T&& filename) {
            return _loadFile(std::forward<T>(filename));
        }

        [[nodiscard]] sol::state& getLuaState() const;

        LuaManager(const LuaManager &) = delete;

        LuaManager &operator=(const LuaManager &) = delete;

        ~LuaManager();
    private:
        static std::unique_ptr<LuaManager> _instance;

        std::unique_ptr<sol::state> _luaState;

        LuaManager() = default;

        bool init();

        bool _loadFile(const std::string& filename);
    };
}


#endif //MOTORSINNOMBREDECIDIDO_LUAMANAGER_H
