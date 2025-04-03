#include "LuaManager.h"
#include "AssetManager.h"
#include <Utils/Error.h>
#include <iostream>
#include <lua.hpp>


lua_State *LuaManager::loadState(char* fileData, int fileLength) {
    lua_State* L = luaL_newstate();
    if (luaL_loadbuffer(L, fileData, fileLength, "filename") != LUA_OK) {
        Error::ShowError("Error al cargar el script Lua",std::string("Error al ejecutar el script Lua: ") + lua_tostring(L, -1));
        return L;
    }
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        Error::ShowError("Error al ejecutar el script Lua", std::string("Error al ejecutar el script Lua: ") + lua_tostring(L, -1));
    }
    return L;
}

Square LuaManager::loadSquare() {
    Square square = {};
    char* fileData = nullptr;
    int fileSize;
    if(AssetManager::readFile("lua/input.lua", fileData, fileSize)) {
        lua_State *L = loadState(fileData, fileSize);
        luaL_openlibs(L);
        lua_getglobal(L, "square");
        if (lua_istable(L, -1)) {
            lua_getfield(L, -1, "color");
            square.color = lua_tointeger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "width");
            square.width = lua_tointeger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "height");
            square.height = lua_tointeger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "speed");
            square.speed = lua_tointeger(L, -1);
            lua_pop(L, 1);

            lua_close(L);
        } else {
            Error::ShowError("Elemento no encontrado",
                                "La tabla 'square' no se encontro en el archivo Lua.");
            lua_close(L);
        }
    }
    return square;
}
