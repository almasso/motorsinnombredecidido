#include "ResourceManager.h"

#include <cassert>
#include <Utils/Error.h>

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

bool ResourceManager::initScenes(sol::table const& config, std::string& scene) {
    scene = config.get_or<std::string>("initScene", "");
    return !scene.empty();
}

bool ResourceManager::Init(std::string const& configFile, std::string& scene, std::string& gameName, std::string& gameIcon) {
    assert(_memoryManager == nullptr);
    LuaReader::Init();
    sol::table config = LuaReader::GetTable(configFile);
    if (!config.valid()) {
        Error::ShowError("Resource initialization", "Could not find a valid configuration file at \"" + configFile + "\"");
        return false;
    }
    if (!initMemoryManager(config))
        return false;
    if (!initScenes(config,scene))
        return false;
    gameName = config.get_or<std::string>("gameName", "Game");
    gameIcon = config.get_or<std::string>("gameIcon", "");
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
