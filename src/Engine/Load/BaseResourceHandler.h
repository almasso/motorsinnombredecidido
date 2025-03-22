#ifndef BASERESOURCEHANDLER_H
#define BASERESOURCEHANDLER_H

class ResourceMemoryManager;

class BaseResourceHandler {
protected:
    ResourceMemoryManager* _resourceMemoryManager;
public:
    BaseResourceHandler();
    virtual ~BaseResourceHandler();
    void init(ResourceMemoryManager* memoryManager);
    void shutdown();
};



#endif //BASERESOURCEHANDLER_H
