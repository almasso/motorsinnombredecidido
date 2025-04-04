//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_PROJECT_H
#define MOTORSINNOMBREDECIDIDO_PROJECT_H

#include <string>
#include <filesystem>
#include <unordered_set>
#include <ctime>
#include <sol/sol.hpp>

namespace editor {
    class Project {
    public:
        template<std::convertible_to<std::string> T>
        Project(T&& name, T&& path, int* dimensions) : _name(std::forward<T>(name)), _projectPath(std::forward<T>(path)) {
            std::copy(dimensions, dimensions + 2, std::begin(_dimensions));
            _found = true;
            updateLastModified();
            saveProject();
        }

        template<std::convertible_to<std::string> T>
        Project(T&& path) {
            updatePath(path);
        }
        const std::string& getName() const;

        template <std::convertible_to<std::string> T>
        void setName(T&& name) {
            _name = std::forward<T>(name);
        }

        template <std::convertible_to<std::string> T>
        void updateName(T&& name) {
            setName(name);
            updateLastModified();
            saveProject();
        }

        const std::filesystem::path& getPath() const;

        template <std::convertible_to<std::string> T>
        void updatePath(T&& path) {
            _projectPath = std::forward<T>(path);
            findProject();
        }

        template <std::convertible_to<std::string> T>
        void addExtraRoute(T&& route) {
            _extraRoutes.insert(std::forward<T>(route));
            saveProject();
        }

        const std::unordered_set<std::filesystem::path>& getExtraRoutes() const;

        const std::tm& getLastModificationTime() const;

        const int* getDimensions() const;

        bool isSetToDelete() const;

        void setToDelete();

        bool isFound() const;

        void updateLastModified();

    private:
        std::string _name;
        std::filesystem::path _projectPath;
        std::unordered_set<std::filesystem::path> _extraRoutes;
        std::tm _lastModified;
        bool _setToDelete = false;
        bool _found = false;

        int _dimensions[2];

        void findProject();

        void loadProject(const sol::table& project);

        void saveProject();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_PROJECT_H
