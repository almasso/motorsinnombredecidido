#ifndef LUAREADER_H
#define LUAREADER_H

#include <string>
#include <sol/sol.hpp>

class LuaReader {
private:
    static LuaReader* _instance;
    sol::state _lua;

    bool init();
    LuaReader();
public:
    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    LuaReader(LuaReader const&) = delete;

    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    void operator=(LuaReader const&) = delete;
    static bool Init();
    static void Shutdown();
    static sol::table GetTable(std::string const& path);
    static sol::table GetTable(sol::table const& container, std::string const& name);
    static sol::function GetFunction(sol::table const& container, std::string const& name);
};


#endif //LUAREADER_H
