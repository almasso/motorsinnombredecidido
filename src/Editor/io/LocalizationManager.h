//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_LOCALIZATIONMANAGER_H
#define MOTORSINNOMBREDECIDIDO_LOCALIZATIONMANAGER_H

#include <memory>
#include <unordered_map>
#include <string>
#include <filesystem>

typedef struct SDL_Locale SDL_Locale;

namespace editor::io {
    class LocalizationManager {
    public:

        static bool Init();

        static LocalizationManager &GetInstance();

        template <std::convertible_to<std::string> T>
        const std::string& getString(T&& name) {
            std::string n = std::forward<T>(name);
            return _stringsTable[n];
        }

        LocalizationManager(const LocalizationManager &) = delete;

        LocalizationManager &operator=(const LocalizationManager &) = delete;

        ~LocalizationManager();
    private:
        static std::unique_ptr<LocalizationManager> _instance;

        SDL_Locale** _locales;

        int _totalUserLocales;

        char* _currentDirectory;

        std::string _preferredLocale;

        std::unordered_map<std::string, std::string> _stringsTable;

        std::filesystem::path _languageTemplatesRoute;

        LocalizationManager() = default;

        bool init();

        void loadLocales();

        bool searchForSecondaryLocales();

        void convertLocalesIntoTable();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_LOCALIZATIONMANAGER_H
