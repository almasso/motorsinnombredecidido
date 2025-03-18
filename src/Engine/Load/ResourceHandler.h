#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#include <unordered_map>

#include "ResourceMemoryManager.h"

class Resource;

template<typename T>
concept resourceDerived = std::is_base_of_v<Resource, T>;

/// @~english
/// @brief Data structure that handles the references to a specific type of resource and its memory.
/// @tparam Key Type used to access the resources.
/// @tparam ResourceType Type of the resource to handle, must be a derived class from \c Resource.
/// @~spanish
/// @brief Estructura de datos que gestiona las referencias a un tipo específico de recursos y su memoria.
/// @tparam Key Tipo usado para acceder a los recursos.
/// @tparam ResourceType Tipo de los recursos a gestionar, debe ser una clase heredada de \c Resource.
template <typename Key, resourceDerived ResourceType>
class ResourceHandler {
private:
    std::unordered_map<Key, ResourceType*> resources_;
    ResourceMemoryManager* resourceMemoryManager_;
public:
    /// @~english
    /// Creates an empty handler.
    /// @~spanish
    /// Crea un gestor vacío.
    inline explicit ResourceHandler(ResourceMemoryManager* resourceMemoryManager) :
        resourceMemoryManager_(resourceMemoryManager) {
    }

    /// @~english
    /// Clears all the resources in the handler.
    /// @~spanish
    /// Elimina todas los recursos del gestor.
    inline ~ResourceHandler() {
        clear();
    }

    /// @~english
    /// @brief Creates and adds to this \c ResourceHandler a new instance of \c ResourceType calling its constructor with the arguments provided.
    /// @tparam Args Types of arguments needed to call the \c ResourceType 's constructor.
    /// @param key Key for later access to the newly added resource.
    /// @param args Arguments needed to call the \c ResourceType 's constructor.
    /// @return Whether the resource creation and addition took place. \c true if it did, \c false if it didn't.
    /// @~spanish
    /// @brief Crea y añade a este \c ResourceHandler una nueva instancia de \c ResourceType llamando a su constructor con los argumentos proporcionados.
    /// @tparam Args Tipos de los argumentos necesarios para llamar al constructor de \c ResourceType .
    /// @param key Clave para acceder posteriormente al nuevo recurso añadido.
    /// @param args Argumentos necesarios para llamar al constructor de \c ResourceType .
    /// @return Si la creación y la adición del recurso ocurrieron. \c true si así fue, \c false si no.
    template <typename ...Args>
    inline bool add(Key const& key, Args&& ...args) {
        auto [it, inserted] = resources_.insert({key, nullptr});
        if (!inserted)
            return false;
        it->second = new ResourceType(std::forward<Args>(args)...);
        return true;
    }

    /// @~english
    /// @brief Adds to this \c ResourceHandler a provided instance of \c ResourceType .
    /// @remarks Be aware that the \c ResourceHandler will try to \c delete the object pointed if that resource is unloaded and \c flush() is called.
    /// @param key Key for later access to the newly added \c Resource .
    /// @param resource A pointer to an already created \c ResourceType allocated in the heap.
    /// @return Whether the resource addition took place. \c true if it did; \c false if it didn't, in that case that resource's memory won't be freed by the \c ResourceHandler .
    /// @~spanish
    /// @brief Añade a este \c ResourceHandler una instancia de \c ResourceType dada.
    /// @remarks A tener en cuenta que el \c ResourceHandler intentará eliminar (\c delete) el objeto apuntado si a ese recurso está descargado y se llama a \c flush() .
    /// @param key Clave para acceder posteriormente al nuevo \c Resource añadido.
    /// @param resource Un puntero a un \c Resource ya creado y localizado en el \a heap.
    /// @return Si la adición del recurso ocurrió. \c true si así fue; \c false si no, en ese caso la memoria de ese recurso no será liberada por el \c ResourceHandler .
    inline bool addReady(Key const& key, ResourceType* resource) {
        return resources_.insert({key, resource}).second;
    }

    /// @~english
    /// @brief Gets the resource linked to a key. Loads it if it wasn't.
    /// @param key Key previously used to add a resource.
    /// @return A pointer to the resource requested. \c nullptr if there isn't any resource saved with that key or if the resource couldn't be loaded.
    /// @~spanish
    /// @brief Accede al recurso asignado a una clave. Lo carga si no lo estaba.
    /// @param key Clave previamente usada para añadir un recurso.
    /// @return Un puntero al recurso solicitado. \c nullptr si no hay ningún recurso guardado con esa clave o si no se pudo cargar el recurso.
    inline ResourceType const* get(Key const& key) {
        auto it = resources_.find(key);
        if (it == resources_.end())
            return nullptr;
        if (!resourceMemoryManager_->activateResource(it->second))
            return nullptr;
        return it->second;
    }

    /// @~english
    /// @brief Unloads all the loaded resources contained in the \c ResourceHandler .
    /// @~spanish
    /// @brief Descarga todos los recursos cargados contenidos en el \c ResourceHandler .
    inline void flush() {
        for (auto it = resources_.begin(); it != resources_.end(); ++it) {
            resourceMemoryManager_->deactivateResource(it->second);
        }
    }

    /// @~english
    /// @brief Frees the memory of every resource and erases them from the handler.
    /// @~spanish
    /// @brief Libera la memoria de todos los recursos y los borra del gestor.
    inline void clear() {
        for (auto it = resources_.begin(); it != resources_.end(); it = resources_.erase(it)) {
            resourceMemoryManager_->deactivateResource(it->second);
            delete it->second;
        }
    }
};


#endif //RESOURCEHANDLER_H
