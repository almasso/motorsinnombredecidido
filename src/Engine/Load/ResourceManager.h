#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <vector>
#include <string>
#include <sol/sol.hpp>

class ResourceMemoryManager;
class BaseResourceHandler;

class ResourceManager {
private:
    static ResourceMemoryManager* _memoryManager;
    static std::vector<BaseResourceHandler*> _handlers;

    static bool initMemoryManager(sol::table const& config);
    static bool initScenes(sol::table const& config, std::string& scene);

public:
    static bool Init(std::string const& configFile, std::string& scene);
    static void Shutdown();
    static void RegisterResourceHandler(BaseResourceHandler* handler);
};



#endif //RESOURCEMANAGER_H
