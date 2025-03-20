#ifndef RESOURCEMEMORYMANAGER_H
#define RESOURCEMEMORYMANAGER_H

#include <cstdint>
#include <list>
#include <unordered_map>

class Resource;

/// @~english
/// @brief Data structure responsible for managing the memory occupied by resources. It ensures that there won't ever be more simultaneous memory used in resources than the assigned maximum size.
/// @~spanish
/// @brief Estructura de datos que se encarga de gestionar la memoria ocupada por recursos. Asegura que nunca se estará usando más memoria en recursos simultáneamente como el tamaño máximo asignado.
class ResourceMemoryManager {
private:
    uint64_t _maxSize;
    uint64_t _currentSize;

    std::list<Resource*> _resources;
    std::unordered_map<Resource*, std::list<Resource*>::iterator> _resourcesIterators;

    /// @~english
    /// @brief Removes stored resources from the manager until there's as much empty space as asked.
    /// @remarks The resources are removed using a Least Recently Used politic.
    /// @param size Size in bytes wanted available.
    /// @return \c false if there's not enough size in the manager to fit the amount asked. \c true otherwise.
    /// @~spanish
    /// @brief Elimina los recursos almacenados en el gestor hasta que haya tanto espacio como el pedido.
    /// @remarks Los recursos se eliminan usando una política <i>Least Recently Used</i> (Menos recientemente usado).
    /// @param size Tamaño en bytes que se quiere disponible.
    /// @return \c false si no hay espacio suficiente en el gestor para encajar el monto solicitado. \c true en caso contrario.
    bool makeRoomForSize(int size);

    /// @~english
    /// @brief Inserts in the manager an already loaded <c>Resource</c>.
    /// @param resource Resource to insert.
    /// @return \c false if the resource was already inserted. \c true otherwise.
    /// @~spanish
    /// @brief Inserta en el gestor un \c Resource ya cargado.
    /// @param resource Recurso a insertar.
    /// @return \c false si el recurso ya estaba insertado. \c true en caso contrario.
    bool insertResource(Resource* resource);
    
public:
    /// @~english
    /// @brief Creates a new <c>ResourceMemoryManager</c>.
    /// @param maxSize Maximum size in bytes to be simultaneously used by resources.
    /// @~spanish
    /// @brief Crea un nuevo <c>ResourceMemoryManager</c>.
    /// @param maxSize Tamaño máximo en bytes a usar simultáneamente por recursos.
    explicit ResourceMemoryManager(uint64_t maxSize);

    /// @~english
    /// @brief Makes sure a resource is loaded. If it was already loaded it will do nothing.
    /// @param resource Resource to activate.
    /// @return \c true on successful activation. \c false if the load failed or the resource can't fit within the maximum size.
    /// @~spanish
    /// @brief Se asegura de que un recurso está cargado. Si ya estaba cargado no hará nada.
    /// @param resource Recurso a activar.
    /// @return \c true si se activa exitosamente. \c false si la carga falló o el recurso no entra dentro del tamaño máximo.
    bool activateResource(Resource* resource);

    /// @~english
    /// @brief Deactivates a previously activated resource. It becomes unloaded and gets removed from the manager.
    /// @remarks This function won't do anything if the resource wasn't activated before.
    /// @param resource Resource to deactivate.
    /// @~spanish
    /// @brief Desactiva un recurso previamente activado. Se descarga y se elimina del gestor.
    /// @remarks Esta función no hará nada si el recurso no se había activado antes.
    /// @param resource Recurso a desactivar.
    void deactivateResource(Resource* resource);
};


#endif //RESOURCEMEMORYMANAGER_H
