#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <cstdint>
#include <vector>

class ResourceMemoryManager;
class BaseResourceHandler;

class ResourceManager {
private:
    static ResourceMemoryManager* _memoryManager;
    static std::vector<BaseResourceHandler*> _handlers;
public:
    static void Init(uint64_t maxSize);
    static void Shutdown();
    static void RegisterResourceHandler(BaseResourceHandler* handler);
};



#endif //RESOURCEMANAGER_H
