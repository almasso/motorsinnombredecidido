#include "ResourceManager.h"

#include <cassert>

#include "ResourceMemoryManager.h"
#include "BaseResourceHandler.h"
#include "LuaReader.h"

ResourceMemoryManager* ResourceManager::_memoryManager = nullptr;
std::vector<BaseResourceHandler*> ResourceManager::_handlers;

bool ResourceManager::initMemoryManager(sol::table const& config) {
    sol::table memory = LuaReader::GetTable(config, "memory");
    if (!memory.valid())
        return false;
    uint64_t maxSize = memory.get_or<uint64_t>("maxSize", 0);
    if (maxSize == 0)
        return false;
    _memoryManager = new ResourceMemoryManager(maxSize);
    return true;
}

bool ResourceManager::initScenes(sol::table const& config) {
    std::string scene = config.get_or<std::string>("initScene", "");
    if (scene == "")
        return false;
    /*TODO: Añadir al scene manager (recibido por parámetro) la escena inicial*/
    return true;
}

bool ResourceManager::Init(std::string const& configFile) {
    assert(_memoryManager == nullptr);
    LuaReader::Init();
    sol::table config = LuaReader::GetTable(configFile);
    if (!config.valid())
        return false;
    if (!initMemoryManager(config))
        return false;
    if (!initScenes(config))
        return false;
    return true;
}

void ResourceManager::Shutdown() {
    for (auto const& handler : _handlers) {
        handler->shutdown();
    }
    _handlers.clear();
    delete _memoryManager;
}

void ResourceManager::RegisterResourceHandler(BaseResourceHandler* handler) {
    assert(_memoryManager != nullptr);
    handler->init(_memoryManager);
    _handlers.push_back(handler);
}
