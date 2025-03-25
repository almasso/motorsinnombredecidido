#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#include <unordered_map>

#include "BaseResourceHandler.h"
#include "ResourceManager.h"
#include "ResourceMemoryManager.h"

class Resource;

template<typename T>
concept resourceDerived = std::is_base_of_v<Resource, T>;

/// @~english
/// @brief Data structure that handles the references to a specific type of resource and its memory.
/// @tparam ResourceType Type of the resource to handle, must be a derived class from \c Resource.
/// @~spanish
/// @brief Estructura de datos que gestiona las referencias a un tipo específico de recursos y su memoria.
/// @tparam ResourceType Tipo de los recursos a gestionar, debe ser una clase heredada de \c Resource.
template <resourceDerived ResourceType>
class ResourceHandler : public BaseResourceHandler {
private:
    std::unordered_map<std::string, ResourceType*> _resources;
    static inline ResourceHandler* _instance = nullptr;

    /// @~english
    /// @brief Creates and adds to this \c ResourceHandler a new instance of \c ResourceType initialized with the given key.
    /// @remarks If a resource with the given key already existed a pointer to that \c ResourceType will be returned and none will be created.
    /// @param key Key for later access to the resource.
    /// @return A pointer the resource assigned to the key provided.
    /// @~spanish
    /// @brief Crea y añade a este \c ResourceHandler una nueva instancia de \c ResourceType inicializado con la clave dada.
    /// @reamrks Si un recurso con la clave dada ya existía se devolverá un puntero a ese \c ResourceType y no se creará uno nuevo.
    /// @param key Clave para acceder posteriormente al recurso.
    /// @return Un puntero al recurso asignado a la clave dada.
    inline ResourceType* add(std::string const& key) {
        auto [it, inserted] = _resources.insert({key, nullptr});
        if (inserted)
            it->second = new ResourceType(key);
        return it->second;
    }

    /// @~english
    /// @brief Deletes the resources assigned to the given key and removes it from the handler.
    /// @remarks If there's no resource assigned to the given key this method will do nothing.
    /// @param key Key previously used to add a resource.
    /// @~spanish
    /// @brief Elimina (<c>delete</c>) el recurso asignado a la clave dada y lo borra del gestor.
    /// @remarks Si no hay un recurso asignado a la clave dada este método no hará nada.
    /// @param key Clave previamente usada para añadir un recurso.
    inline void remove(std::string const& key) {
        auto it = _resources.find(key);
        if (it == _resources.end())
            return;
        delete it->second;
        _resources.erase(key);
    }

    /// @~english
    /// @brief Creates an empty \c ResourceHandler .
    /// @~spanish
    /// @brief Crea un \c ResourceHandler vacío.
    inline ResourceHandler() = default;

public:
    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    ResourceHandler(ResourceHandler const&) = delete;

    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    void operator=(ResourceHandler const&) = delete;

    /// @~english
    /// @brief Access to the single instance of the \c ResourceHandler.
    /// @return Pointer to the single instance of the \c ResourceHandler.
    /// @~spanish
    /// @brief Accede a la instancia única del \c ResourceHandler.
    /// @return Puntero a la instancia única del \c ResourceHandler.
    static inline ResourceHandler* Instance() {
        if (_instance != nullptr)
            return _instance;
        _instance = new ResourceHandler();
        ResourceManager::RegisterResourceHandler(_instance);
        return _instance;
    }

    /// @~english
    /// @brief Clears all the resources in the handler.
    /// @~spanish
    /// @brief Elimina todos los recursos del gestor.
    inline ~ResourceHandler() override {
        clear();
    }

    /// @~english
    /// @brief Gets the resource linked to a key. Loads it if it wasn't.
    /// @param key Key assigned to the wanted \c ResourceType.
    /// @return A pointer to the resource requested. \c nullptr if there isn't any resource saved with that key or if the resource couldn't be loaded.
    /// @~spanish
    /// @brief Accede al recurso asignado a una clave. Lo carga si no lo estaba.
    /// @param key Clave previamente usada para añadir un recurso.
    /// @return Un puntero al recurso solicitado. \c nullptr si no hay ningún recurso guardado con esa clave o si no se pudo cargar el recurso.
    inline ResourceType const* get(std::string const& key) {
        ResourceType* resource = add(key);
        if (!_resourceMemoryManager->activateResource(resource)) {
            remove(key);
            return nullptr;
        }
        return resource;
    }

    /// @~english
    /// @brief Frees the memory of every resource and erases them from the handler.
    /// @~spanish
    /// @brief Libera la memoria de todos los recursos y los borra del gestor.
    inline void clear() {
        for (auto it = _resources.begin(); it != _resources.end(); it = _resources.erase(it)) {
            _resourceMemoryManager->deactivateResource(it->second);
            delete it->second;
        }
    }
};


#endif //RESOURCEHANDLER_H
