//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_LUAMANAGER_H
#define MOTORSINNOMBREDECIDIDO_LUAMANAGER_H

#include <memory>
#include <lua.hpp>

//typedef struct lua_State lua_State;

namespace editor::io {
    class LuaManager {
    public:
        static bool Init();

        static LuaManager& GetInstance();

        template <std::convertible_to<std::string> T>
        bool loadFile(T&& filename) {
            std::string fn = std::forward<T>(filename);
            if(luaL_dofile(_luaState, fn.c_str())) {
                // Error con lua_tostring(_luaState, -1)
                lua_pop(_luaState, 1);
                return false;
            }
            return true;
        }

        lua_State* getLuaState() const;

        LuaManager(const LuaManager &) = delete;

        LuaManager &operator=(const LuaManager &) = delete;

        ~LuaManager();
    private:
        static std::unique_ptr<LuaManager> _instance;

        lua_State* _luaState;

        LuaManager() = default;

        bool init();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_LUAMANAGER_H
