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

    /// @~english
    /// @brief Initializes the \c ResourceMemoryManager given a configuration.
    /// @param config Reference to the open configuration file.
    /// @return \c true if there's a valid memory configuration in the configuration file. \c false otherwise.
    /// @~spanish
    /// @brief Inicializa el \c ResourceMemoryManager dada una configuración.
    /// @param config Referencia al archivo de configuración abierto.
    /// @return \c true si hay una configuración de memoria válida en el archivo de configuración. \c false si no.
    static bool initMemoryManager(sol::table const& config);

    /// @~english
    /// @brief Stores the initial scene's name given in the configuration.
    /// @param config Reference to the open configuration file.
    /// @param scene Our parameter to store the initial scene's name.
    /// @return \c true if there's a valid scene name in the configuration file. \c false otherwise.
    /// @~spanish
    /// @brief Almacena el nombre de la escena inicial dado en la configuración.
    /// @param config Referencia al archivo de configuración abierto.
    /// @param scene Parámetro de salida para almacenar el nombre de la escena inicial.
    /// @return \c true si hay un nombre de escena válido en el archivo de configuración. \c false si no.
    static bool initScenes(sol::table const& config, std::string& scene);

public:
    /// @~english
    /// @brief Initializes the \c ResourceManager with a given configuration file.
    /// @param configFile Path to a configuration file.
    /// @param scene Out parameter to store the initial scene given in the configuration file.
    /// @return \c true on successful initialization. \c false if the configuration file was not okay.
    /// @~spanish
    /// @brief Inicializa el \c ResourceManager con un archivo de configuración dado.
    /// @param configFile Ruta al archivo de configuración.
    /// @param scene Parámetro de salida para almacenar la escena inicial dada en el archivo de configuración.
    /// @return \c true en inicialización exitosa. \c false si el archivo de configuración no estaba bien.
    static bool Init(std::string const& configFile, std::string& scene, std::string& gameName, std::string& gameIcon);

    /// @~english
    /// @brief Unregisters every registered \c BaseResourceHandler and stops the \c ResourceManager 's work.
    /// @~spanish
    /// @brief Borra todos los \c BaseResourceHandler registrados y detiene el trabajo del \c ResourceManager .
    static void Shutdown();

    /// @~english
    /// @brief Registers a \c BaseResourceHandler to initialize and store.
    /// @param handler Pointer to an instance of \c BaseResourceHandler .
    /// @~spanish
    /// @brief Registra un \c BaseResourceHandler a inicializar y almacenar.
    /// @param handler Puntero a una instancia de \c BaseResourceHandler .
    static void RegisterResourceHandler(BaseResourceHandler* handler);
};



#endif //RESOURCEMANAGER_H
