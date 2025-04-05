//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef TILE_H
#define TILE_H

#include <string>

typedef unsigned long long ImU64;
typedef ImU64 ImTextureID;

namespace editor::resources {

struct Tile {
    std::string tileset;
    int pos;
    ImTextureID texture;
};

}

#endif //TILE_H
