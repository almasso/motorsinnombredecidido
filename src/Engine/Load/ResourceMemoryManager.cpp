#include "ResourceMemoryManager.h"
#include "Resource.h"


bool ResourceMemoryManager::makeRoomForSize(int size) {
    if (size > _maxSize)
        return false;
    for (auto it = _resources.begin();
        it != _resources.end() && _currentSize + size > _maxSize;
        it = _resources.erase(it)) {

        (*it)->unload();
        _resourcesIterators.erase(*it);
        _currentSize -= (*it)->getSize();
    }
    return true;
}

bool ResourceMemoryManager::insertResource(Resource* resource) {
    if (_resourcesIterators.contains(resource))
        return false;
    _resourcesIterators.insert({resource, _resources.insert(_resources.end(), resource)});
    _currentSize += resource->getSize();
    return true;
}

ResourceMemoryManager::ResourceMemoryManager(uint64_t maxSize) :
    _maxSize(maxSize),
    _currentSize(0) {
}

bool ResourceMemoryManager::activateResource(Resource* resource) {
    if (auto it = _resourcesIterators.find(resource); it != _resourcesIterators.end()) {
        _resources.splice(_resources.end(), _resources, it->second);
        return true;
    }
    if (!resource->load())
        return false;
    if (_currentSize + resource->getSize() >= _maxSize &&
        !makeRoomForSize(resource->getSize())) {
        resource->unload();
        return false;
    }
    if (!insertResource(resource)) {
        resource->unload();
        return false;
    }
    return true;
}

void ResourceMemoryManager::deactivateResource(Resource* resource) {
    auto it = _resourcesIterators.find(resource);
    if (it == _resourcesIterators.end())
        return;
    _resources.erase(it->second);
    _resourcesIterators.erase(it);
    _currentSize -= resource->getSize();
    resource->unload();
}
