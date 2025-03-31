//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_PREFERENCESMANAGER_H
#define MOTORSINNOMBREDECIDIDO_PREFERENCESMANAGER_H

#include <memory>
#include <filesystem>
#include <sol/sol.hpp>
#include <any>
#include <string>

namespace editor::io {
    class PreferencesManager {
    public:
        static bool Init();

        static void Destroy();

        static PreferencesManager& GetInstance();

        template <std::convertible_to<std::string> T, typename Value>
        void setPreference(T&& preferenceName, Value&& value) {
            std::string prefName = std::forward<T>(preferenceName);
            _preferences[prefName] = std::forward<Value>(value);
        }

        template <typename Value, typename T> requires std::convertible_to<T, std::string>
        std::optional<Value> getPreference(T&& preferenceName) const {
            std::string prefName = std::forward<T>(preferenceName);
            sol::object pref = _preferences[prefName];
            if(pref.valid()) {
                try {
                    return pref.as<Value>();
                } catch(const sol::error&) {
                    return std::nullopt;
                }
            }
            return std::nullopt;
        }

        static void Dump();

        PreferencesManager(const PreferencesManager&) = delete;

        PreferencesManager& operator=(const PreferencesManager&) = delete;

        ~PreferencesManager();
    private:
        static std::unique_ptr<PreferencesManager> _instance;

        std::filesystem::path _preferencesPath;

        char* _currentDirectory = nullptr;

        sol::table _preferences;

        PreferencesManager() = default;

        bool init();

        void loadPreferences();

        void savePreferences();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_PREFERENCESMANAGER_H
