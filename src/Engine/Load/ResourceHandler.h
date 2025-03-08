#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#include <unordered_map>

/// @~english
/// @brief Data structure that handles the references to a specific type of resource and its memory.
/// @tparam Key Type used to access the resources.
/// @tparam Resource Type of the resource to handle.
/// @~spanish
/// @brief Estructura de datos que gestiona las referencias a un tipo específico de recursos y su memoria.
/// @tparam Key Tipo usado para acceder a los recursos.
/// @tparam Resource Tipo de los recursos a gestionar.
template <typename Key, typename Resource>
class ResourceHandler {
private:
    struct ResourceReference {
        Resource const* resource;
        int referenceCount;
    };
    std::unordered_map<Key, ResourceReference> resources_;

public:
    /// @~english
    /// Creates an empty handler.
    /// @~spanish
    /// Crea un gestor vacío.
    inline ResourceHandler() = default;

    /// @~english
    /// Clears all the resources in the handler.
    /// @~spanish
    /// Elimina todas los recursos del gestor.
    inline ~ResourceHandler() {
        clear();
    }

    /// @~english
    /// @brief Creates and adds to this \c ResourceHandler a new instance of \c Resource calling its constructor with the arguments provided.
    /// @tparam Args Types of arguments needed to call the \c Resource 's constructor.
    /// @param key Key for later access to the newly added \c Resource.
    /// @param args Arguments needed to call the \c Resource 's constructor.
    /// @return Whether the resource creation and addition took place. \c true if it did, \c false if it didn't.
    /// @~spanish
    /// @brief Crea y añade a este \c ResourceHandler una nueva instancia de \c Resource llamando a su constructor con los argumentos proporcionados.
    /// @tparam Args Tipos de los argumentos necesarios para llamar al constructor de \c Resource .
    /// @param key Clave para acceder posteriormente al nuevo \c Resource añadido.
    /// @param args Argumentos necesarios para llamar al constructor de \c Resource .
    /// @return Si la creación y la adición del recurso ocurrieron. \c true si así fue, \c false si no.
    template <typename ...Args>
    inline bool add(Key const& key, Args&& ...args) {
        auto [it, inserted] = resources_.insert({key, {nullptr, 0}});
        if (!inserted)
            return false;
        it->second.resource = new Resource(std::forward<Args>(args)...);
        return true;
    }

    /// @~english
    /// @brief Adds to this \c ResourceHandler a provided instance of \c Resource .
    /// @remarks Be aware that the \c ResourceHandler will try to \c delete the object pointed if that \c Resource has no references left and \c flush() is called.
    /// @param key Key for later access to the newly added \c Resource .
    /// @param resource A pointer to an already created \c Resource allocated in the heap.
    /// @return Whether the resource addition took place. \c true if it did; \c false if it didn't, in that case that \c Resource 's memory won't be freed by the \c ResourceHandler .
    /// @~spanish
    /// @brief Añade a este \c ResourceHandler una instancia de \c Resource dada.
    /// @remarks A tener en cuenta que el \c ResourceHandler intentará eliminar (\c delete) el objeto apuntado si a ese \c Resource no le quedan referencias y se llama a \c flush() .
    /// @param key Clave para acceder posteriormente al nuevo \c Resource añadido.
    /// @param resource Un puntero a un \c Resource ya creado y localizado en el \a heap.
    /// @return Si la adición del recurso ocurrió. \c true si así fue; \c false si no, en ese caso la memoria de ese \c Resource no será liberada por el \c ResourceHandler .
    inline bool addReady(Key const& key, Resource const* resource) {
        return resources_.insert({key, {resource, 0}}).second;
    }

    /// @~english
    /// @brief Gets the resource linked to a key and takes count of a new reference to that resource.
    /// @remarks Once the work with the resource requested is done programmer \b must call <c>dereference(Key const& key)</c>.
    /// @param key Key previously used to add a resource.
    /// @return A pointer to the resource requested. \c nullptr if there isn't any resource saved with that key.
    /// @~spanish
    /// @brief Accede al recurso asignado a una clave y tiene en cuenta una nueva referencia a ese recurso.
    /// @remarks Una vez el trabajo con el recurso solicitado se ha terminado el programador \b debe llamar a <c>dereference(Key const& key)</c>.
    /// @param key Clave previamente usada para añadir un recurso.
    /// @return Un puntero al recurso solicitado. \c nullptr si no hay ningún recurso guardado con esa clave.
    inline Resource const* get(Key const& key) {
        auto it = resources_.find(key);
        if (it == resources_.end())
          return nullptr;
        ++it->second.referenceCount;
        return it->second.resource;
    }

    /// @~english
    /// @brief Gets the number of references to a specific resource.
    /// @param key Key previously used to add a resource.
    /// @return Number of references to a specific resource. \c -1 if there's no resource with the given key.
    /// @~spanish
    /// @brief Accede al número de referencias de un recurso específico.
    /// @param key Clave previamente usada para añadir un recurso.
    /// @return Número de referencias a un recurso específico. \c -1 si no hay recursos guardados con la clave dada.
    inline int getReferenceCount(Key const& key) const {
        auto it = resources_.find(key);
        if (it == resources_.end())
            return -1;
        return it->second.referenceCount;
    }

    /// @~english
    /// @brief Marks that the work with a reference to a previously requested resource is done.
    /// @remark Programmer should not expect the dereferenced resource to work properly. Call again <c>get(Key const& key)</c> to recover access. Be aware that before you try to get a reference back the resource will already be deleted if no references where left and \c flush() was called.
    /// @param key Key previously used to add a resource.
    /// @return Number of references left to the resource after dereferencing. \c -1 if there's no resource with the given key.
    /// @~spanish
    /// @brief Marca que el trabajo con una referencia a un recurso previamente solicitado se ha terminado.
    /// @remark El programador no debería esperar que el recurso desreferenciado funcione correctamente. Para recuperar el acceso se ha de llamar a <c>get(Key const& key)</c>. A tener en cuenta que antes de que se intente recuperar una referencia el recurso se habrá eliminado si no quedaban referencias y se llamó a <c>flush()</c>.
    /// @param key Clave previamente usada para añadir un recurso.
    /// @return Número de referencias restantes tras desreferenciar el recurso. \c -1 si no hay ningún recurso guardado con la clave dada.
    inline int dereference(Key const& key) {
        auto it = resources_.find(key);
        if (it == resources_.end())
          return -1;
        --it->second.referenceCount;
        return it->second.referenceCount;
    }

    /// @~english
    /// @brief Frees the memory of every resource with no references and erases them from the handler.
    /// @return Number of resources erased.
    /// @~spanish
    /// @brief Libera la memoria de todos los recursos que no tengan referencias y lo borra del gestor.
    /// @return Número de recursos borrados.
    inline int flush() {
        int initAmount = resources_.size();
        for (auto it = resources_.begin(); it != resources_.end();) {
            if (it->second.referenceCount > 0)
                ++it;
            else {
                delete it->second.resource;
                it = resources_.erase(it);
            }
        }
        return resources_.size() - initAmount;
    }

    /// @~english
    /// @brief Frees the memory of every resource and erases them from the handler.
    /// @~spanish
    /// @brief Libera la memoria de todos los recursos y los borra del gestor.
    inline void clear() {
        for (auto it = resources_.begin(); it != resources_.end(); it = resources_.erase(it))
            delete it->second.resource;
    }
};


#endif //RESOURCEHANDLER_H
