#include "ResourceManager.h"

#include <cassert>

#include "ResourceMemoryManager.h"
#include "BaseResourceHandler.h"

ResourceMemoryManager* ResourceManager::_memoryManager = nullptr;
std::vector<BaseResourceHandler*> ResourceManager::_handlers;

void ResourceManager::Init(uint64_t maxSize) {
    assert(_memoryManager == nullptr);
    _memoryManager = new ResourceMemoryManager(maxSize);
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
