#include "ResourceMemoryManager.h"
#include "Resource.h"


bool ResourceMemoryManager::makeRoomForSize(int size) {
    if (size > maxSize_)
        return false;
    for (auto it = resources_.begin();
        it != resources_.end() && currentSize_ + size > maxSize_;
        it = resources_.erase(it)) {

        (*it)->unload();
        resourcesIterators_.erase(*it);
        currentSize_ -= (*it)->getSize();
    }
    return true;
}

bool ResourceMemoryManager::insertResource(Resource* resource) {
    if (resourcesIterators_.contains(resource))
        return false;
    resourcesIterators_.insert({resource, resources_.insert(resources_.end(), resource)});
    currentSize_ += resource->getSize();
    return true;
}

ResourceMemoryManager::ResourceMemoryManager(uint64_t maxSize) :
    maxSize_(maxSize),
    currentSize_(0) {
}

bool ResourceMemoryManager::activateResource(Resource* resource) {
    if (auto it = resourcesIterators_.find(resource); it != resourcesIterators_.end()) {
        resources_.splice(resources_.end(), resources_, it->second);
        return true;
    }
    if (!resource->load())
        return false;
    if (currentSize_ + resource->getSize() >= maxSize_ &&
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
    auto it = resourcesIterators_.find(resource);
    if (it == resourcesIterators_.end())
        return;
    resources_.erase(it->second);
    resourcesIterators_.erase(it);
    currentSize_ -= resource->getSize();
    resource->unload();
}
