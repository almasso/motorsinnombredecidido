//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Map.h"

#include <map>
#include <map>
#include <io/LuaManager.h>

#include "Object.h"
#include "Tile.h"
#include "common/Project.h"
#include "Tileset.h"
#include <Utils/Vector2.h>
#define widthKey "width"
#define heightKey "height"
#define layersKey "layers"
#define tilesKey "tiles"
#define collisionsKey "collisions"
#define objectsKey "objects"

#define tilesetKey "tileset"
#define posKey "pos"

std::filesystem::path editor::resources::Map::_mapsDirectory;

editor::resources::Map::Map(Project* project) :
    EditorResource("map"),
    _mapWidth(0),
    _mapHeight(0),
    _mapX(0),
    _mapY(0),
    _layers(0),
    _project(project){
}

editor::resources::Map::~Map() {
    for (auto& [pos, object] : _objects) {
        delete object;
        object = nullptr;
    }
    _objects.clear();
    _collisions.clear();
    _tiles.clear();
}

void editor::resources::Map::init(std::string const& name, int mapWidth, int mapHeight, int layers) {
    _name = name;
    _mapWidth = mapWidth;
    _mapHeight = mapHeight;
    _layers = layers;
    _name = name;
    handleVectorChanges();
    _init = true;
}

void editor::resources::Map::handleVectorChanges() {
    // Objetos
    if(!_objects.empty()) {
        std::unordered_map<int, Object*> objTmp;
        int count = 0;
        for(auto it = _objects.begin(); it != _objects.end() && count < _mapWidth * _mapHeight;) {
            objTmp[it->first] = it->second;
            it = _objects.erase(it);
            ++count;
        }

        for(auto& obj : _objects) delete obj.second;
        _objects.clear();
        _objects = std::move(objTmp);
    }

    // Tiles
    if(!_tiles.empty()) {
        std::vector<std::vector<Tile*>> tilesTmp = std::vector(_layers, std::vector<Tile*>(_mapWidth * _mapHeight, nullptr));
        int minLayers = std::min(tilesTmp.size(), _tiles.size());
        for(int i = 0; i < minLayers; ++i) {
            int minGridSize = std::min(tilesTmp[i].size(), _tiles[i].size());
            for(int j = 0; j < minGridSize; ++j) {
                tilesTmp[i][j] = _tiles[i][j];
                _tiles[i][j] = nullptr;
            }
            for(int k = minGridSize; k < _tiles[i].size(); ++k) {
                delete _tiles[i][k];
                _tiles[i][k] = nullptr;
            }
        }
        for(int k = minLayers; k < _tiles.size(); ++k) {
            for(Tile* tile : _tiles[k]) delete tile;
        }
        _tiles.clear();
        _tiles = std::move(tilesTmp);
    }
    else _tiles = std::vector(_layers, std::vector<Tile*>(_mapWidth * _mapHeight, nullptr));

    // Collisions
    if(!_collisions.empty()) {
        std::vector<bool> collTmp = std::vector(_mapWidth * _mapHeight, false);

        int minSize = std::min(_collisions.size(), collTmp.size());
        for(int i = 0; i < minSize; ++i) {
            collTmp[i] = _collisions[i];
        }
        _collisions.clear();
        _collisions = std::move(collTmp);
    }
    else _collisions = std::vector(_mapWidth * _mapHeight, false);
}

bool editor::resources::Map::readFromLua(std::string const& name) {
    sol::table mapTable = io::LuaManager::GetInstance().getTable(GetFilePath(name));
    if (!mapTable.valid())
        return false;

    sol::optional<int> width = mapTable.get<sol::optional<int>>(widthKey);
    if (!width.has_value())
        return false;

    sol::optional<int> height = mapTable.get<sol::optional<int>>(heightKey);
    if (!height.has_value())
        return false;

    sol::optional<int> layers = mapTable.get<sol::optional<int>>(layersKey);
    if (!layers.has_value())
        return false;

    init(name, width.value(), height.value(), layers.value());

    sol::optional<sol::table> tiles = mapTable.get<sol::optional<sol::table>>(tilesKey);
    if (!tiles.has_value())
        return false;
    if (!readTiles(*tiles))
        return false;

    sol::optional<sol::table> collisions = mapTable.get<sol::optional<sol::table>>(collisionsKey);
    if (!collisions.has_value())
        return false;
    if (!readCollisions(*collisions))
        return false;

    sol::optional<sol::table> objects = mapTable.get<sol::optional<sol::table>>(objectsKey);
    if (!objects.has_value())
        return false;
    if (!readObjects(*objects))
        return false;

    return true;
}


void editor::resources::Map::writeToLua() {
    auto& lua = io::LuaManager::GetInstance().getState();
    sol::table map = lua.create_table();
    map[widthKey] = _mapWidth;
    map[heightKey] = _mapHeight;
    map[layersKey] = _layers;

    sol::table tiles = lua.create_table();
    writeTiles(tiles);
    map[tilesKey] = tiles;

    sol::table collisions = lua.create_table();
    writeCollisions(collisions);
    map[collisionsKey] = collisions;

    sol::table objects = lua.create_table();
    writeObjects(objects);
    map[objectsKey] = objects;

    io::LuaManager::GetInstance().writeToFile(map, GetFilePath(_name));
}

void editor::resources::Map::writeToEngineLua(const std::string &platform) {
    auto& lua = io::LuaManager::GetInstance().getState();
    sol::table map = lua.create_table();
    sol::table children = lua.create_table();
    sol::table components = lua.create_table();
    writeComponents(components);
    map["components"] = components;
    writeChildren(children);
    map["children"] = children;
    io::LuaManager::GetInstance().writeToFile(map, (_project->getBuildPath(platform)/"data"/"prefabs"/(_name+".lua")).string());
}

void editor::resources::Map::writeComponents(sol::table &components) {
    auto& lua = io::LuaManager::GetInstance().getState();
    Vector2 dimensions = Vector2(_project->getDimensions()[0], _project->getDimensions()[1]);
    sol::table transform = lua.create_table();
    transform["position"] = sol::as_table<std::array<float,2>>(Vector2(_mapX + _mapWidth/2.0f, _mapY + _mapHeight/2.0f) * dimensions);
    components["Transform"] = transform;
    sol::table collider = lua.create_table();
    collider["size"] = sol::as_table<std::array<float,2>>(Vector2(_mapWidth, _mapHeight) * dimensions);
    components["Collider"] = collider;
    sol::table mapComponent = lua.create_table();
    mapComponent["adjacentMaps"] = sol::as_table(_adjacent);
    components["MapComponent"] = mapComponent;
}

void editor::resources::Map::writeChildren(sol::table &children) {
    auto& lua = io::LuaManager::GetInstance().getState();
    Vector2 dimensions = Vector2(_project->getDimensions()[0], _project->getDimensions()[1]);
    Vector2 center = Vector2(_mapWidth/2.0f, _mapHeight/2.0f);
    std::unordered_set<int> collisions;
    for(int i = 0; i < _collisions.size(); ++i) {
        if (_collisions[i]) collisions.insert(i);
    }
    for (int i = 0; i < _layers; i++) {
        for (int j = 0; j < _mapWidth; j++) {
            for (int k = 0; k < _mapHeight; k++) {
                Tile* tile = _tiles[i][k * _mapWidth + j];
                if (tile != nullptr || (collisions.contains(k * _mapWidth + j) && i == _layers-1)) {
                    sol::table child = lua.create_table();
                    sol::table components = lua.create_table();
                    sol::table transform = lua.create_table();
                    transform["position"] = sol::as_table<std::array<float,2>>((Vector2(j, k) - center) * dimensions);
                    components["Transform"] = transform;
                    if (tile != nullptr) {
                        sol::table sprite = lua.create_table();
                        sprite["sprite"] = tile->tileset.c_str() + tile->pos;
                        sprite["layer"] = i;
                        components["SpriteRenderer"] = sprite;
                    }
                    if (auto finder = collisions.find(k * _mapWidth + j); finder != collisions.end()) {
                        components["MovementObstacle"] = {0};
                        collisions.erase(finder);
                    }
                    child["components"] = components;
                    std::string childKey = "tile"+i+j+k;
                    children[childKey] = child;
                }
            }
        }
    }
}

bool editor::resources::Map::isAdjacent(const Map* other) const {
    int aLeft   = this->_mapX;
    int aRight  = this->_mapX + this->_mapWidth;
    int aTop    = this->_mapY;
    int aBottom = this->_mapY + this->_mapHeight;

    int bLeft   = other->_mapX;
    int bRight  = other->_mapX + other->_mapWidth;
    int bTop    = other->_mapY;
    int bBottom = other->_mapY + other->_mapHeight;

    if ((aRight == bLeft || aLeft == bRight) &&
        !(aBottom <= bTop || aTop >= bBottom)) {
        return true;
    }
    if ((aBottom == bTop || aTop == bBottom) &&
        !(aRight <= bLeft || aLeft >= bRight)) {
        return true;
    }
    return false;
}

void editor::resources::Map::setAdjacent(const std::vector<std::string> &adjacent) {
    _adjacent = adjacent;
}


std::vector<std::vector<editor::resources::Tile*>> &editor::resources::Map::getTiles() {
    return _tiles;
}

const std::string &editor::resources::Map::getName() const {
    return _name;
}

std::vector<bool> &editor::resources::Map::getCollisions() {
    return _collisions;
}

int editor::resources::Map::getMapWidth() const {
    return _mapWidth;
}

int editor::resources::Map::getMapHeight() const {
    return _mapHeight;
}

int editor::resources::Map::getMapX() const {
    return _mapX;
}

int editor::resources::Map::getMapY() const {
    return _mapY;
}

void editor::resources::Map::setMapX(int x) {
    _mapX = x;
}

void editor::resources::Map::setMapY(int y) {
    _mapY = y;
}

int editor::resources::Map::getLayers() const {
    return _layers;
}

void editor::resources::Map::addLayer() {
    _layers++;
    _tiles.push_back(std::vector<Tile*>(_mapWidth * _mapHeight, nullptr));
}

void editor::resources::Map::removeLayer(int index) {
    _tiles.erase(_tiles.begin() + index);
    _layers--;
}


void editor::resources::Map::SetMapsDirectory(std::filesystem::path const& mapsDirectory) {
    _mapsDirectory = mapsDirectory;
}


void editor::resources::Map::writeTiles(sol::table& tiles) {
    for (auto const& row : _tiles) {
        sol::table rowTable = io::LuaManager::GetInstance().getState().create_table();
        for (auto const& tile : row) {
            sol::table tileTable = io::LuaManager::GetInstance().getState().create_table();
            if (tile != nullptr) {
                tileTable[tilesetKey] = tile->tileset;
                tileTable[posKey] = tile->pos;
            }
            rowTable.add(tileTable);
        }
        tiles.add(rowTable);
    }
}

void editor::resources::Map::writeCollisions(sol::table& collisions) {
    for (bool const& collision : _collisions) {
        collisions.add(collision);
    }
}

bool editor::resources::Map::writeObjects(sol::table& objects) {
    for (auto const& [key, object] : _objects) {
        sol::table objectTable = io::LuaManager::GetInstance().getState().create_table();
        if (!object->write(objectTable))
            return false;
        objects.add(objectTable);
    }
    return true;
}


bool editor::resources::Map::readTiles(sol::table const& tiles) {
    auto rowIt = _tiles.begin();
    for (auto const& [key, row] : tiles) {
        if (rowIt == _tiles.end())
            return false;

        if (!row.is<sol::table>())
            return false;
        sol::table rowTable = row.as<sol::table>();

        auto tileIt = rowIt->begin();
        for (auto const& [key, tile] : rowTable) {
            if (tileIt == rowIt->end())
                return false;

            if (!tile.is<sol::table>())
                return false;
            sol::table tileTable = tile.as<sol::table>();

            if (tileTable.empty()) {
                ++tileIt;
                continue;
            }

            auto tileset = tileTable.get_or<std::string>(tilesetKey, "");
            if (tileset.empty())
                return false;

            auto pos = tileTable.get_or<int, std::string, int>(posKey, -1);
            if (pos < 0)
                return false;

            (*tileIt) = _project->getTileset(tileset)->getTiles()[pos];

            ++tileIt;
        }
        ++rowIt;
    }
    return true;
}

bool editor::resources::Map::readCollisions(sol::table const& collisions) {
    auto collisionIt = _collisions.begin();
    for (const auto& [key, collision] : collisions) {
        if (collisionIt == _collisions.end())
            return false;
        if (!collision.is<bool>())
            return false;
        (*collisionIt) = collision.as<bool>();
        ++collisionIt;
    }
    return true;
}

bool editor::resources::Map::readObjects(sol::table const& objects) {
    for (const auto& [key, object] : objects) {
        auto obj = new Object();
        if (!obj->read(object)) {
            delete obj;
            return false;
        }
        _objects.insert({obj->getX() + obj->getY() * _mapWidth, obj});
    }
    return true;
}


std::string editor::resources::Map::GetFilePath(std::string const& mapName) {
    return (_mapsDirectory / (mapName + ".lua")).string();
}

bool editor::resources::Map::isInitialized() const {
    return _init;
}
