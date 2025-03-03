//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_RESOURCEMANAGER_H
#define MOTORSINNOMBREDECIDIDO_RESOURCEMANAGER_H

#include <memory>

namespace editor::io {
    class ResourceManager {
    public:
        static bool Init();
        static ResourceManager &GetInstance();
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;
        ~ResourceManager();
    private:
        static std::unique_ptr<ResourceManager> _instance;

        ResourceManager() = default;

        bool init();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_RESOURCEMANAGER_H
