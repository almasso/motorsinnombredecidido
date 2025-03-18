#ifndef RESOURCEMEMORYMANAGER_H
#define RESOURCEMEMORYMANAGER_H

#include <cstdint>
#include <list>
#include <unordered_map>

class Resource;

class ResourceMemoryManager {
private:
    uint64_t maxSize_;
    uint64_t currentSize_;

    std::list<Resource*> resources_;
    std::unordered_map<Resource*, std::list<Resource*>::iterator> resourcesIterators_;

    bool makeRoomForSize(int size);
    bool insertResource(Resource* resource);
public:
    explicit ResourceMemoryManager(uint64_t maxSize);
    bool activateResource(Resource* resource);
    void deactivateResource(Resource* resource);
};


#endif //RESOURCEMEMORYMANAGER_H
