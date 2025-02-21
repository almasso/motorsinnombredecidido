#ifndef LUAMANAGER_H
#define LUAMANAGER_H

struct Square {
     int color = 0xFF00FF;
     float
     width = 250,
     height = 250,
     speed = 1;
};

class AssetManager;
class lua_State;

class LuaManager {
 public:
     static Square loadSquare();
     static lua_State* loadState(char* fileData, int fileLength);
};



#endif //LUAMANAGER_H
