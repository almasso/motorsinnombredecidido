//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_LOCALIZATIONMANAGER_H
#define MOTORSINNOMBREDECIDIDO_LOCALIZATIONMANAGER_H

#include <memory>

namespace editor::io {
    class LocalizationManager {
    public:
        static bool Init();
        static LocalizationManager &GetInstance();
        LocalizationManager(const LocalizationManager &) = delete;
        LocalizationManager &operator=(const LocalizationManager &) = delete;
        ~LocalizationManager();
    private:
        static std::unique_ptr<LocalizationManager> _instance;

        LocalizationManager() = default;

        bool init();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_LOCALIZATIONMANAGER_H
