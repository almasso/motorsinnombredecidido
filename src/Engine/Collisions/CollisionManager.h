#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Collider.h"

class CollisionManager {
private:
    std::unordered_set<Collider*> _colliders;

    static CollisionManager* _instance;

    /// @~english
    /// @brief Handles the state change of \c collider when it is colliding with \c collider2.
    /// @param collider \c Collider to update the state of.
    /// @param collider2 \c Collider to compare with the first one.
    /// @~spanish
    /// @brief Gestiona el cambio de estado de \c collider cuando está colisionando con \c collider2.
    /// @param collider \c Collider a actualizar su estado.
    /// @param collider2 \c Collider a comparar con el primero.
    void handleCollision(Collider* collider, Collider* collider2);

    /// @~english
    /// @brief Handles the state change of \c collider when it isn't colliding with \c collider2.
    /// @param collider \c Collider to update the state of.
    /// @param collider2 \c Collider to compare with the first one.
    /// @~spanish
    /// @brief Gestiona el cambio de estado de \c collider cuando no está colisionando con \c collider2.
    /// @param collider \c Collider a actualizar su estado.
    /// @param collider2 \c Collider a comparar con el primero.
    void handleCollisionEnd(Collider* collider, Collider* collider2);

    /// @~english
    /// @brief Updates the collision state of collider with collider2.
    /// @param collider \c Collider to update the state of.
    /// @param collider2 \c Collider to compare with the first one.
    /// @~spanish
    /// @brief Actualiza el estado de colisión de \a collider con \a collider2.
    /// @param collider \c Collider a actualizar su estado.
    /// @param collider2 \c Collider a comparar con el primero.
    void updateCollisions(Collider* collider, Collider* collider2);

    /// @~english
    /// @brief Creates an empty \c CollisionManager .
    /// @~spanish
    /// @brief Crea un \c CollisionManager vacío.
    CollisionManager();

public:
    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    CollisionManager(CollisionManager const&) = delete;

    /// @~english
    /// @brief Copy possibility deletion for singleton pattern assurance
    /// @~spanish
    /// @brief Eliminación de la posibilidad de copia para asegurar el patrón \a singleton.
    void operator=(CollisionManager const&) = delete;

    /// @~english
    /// @brief Stops the collision system.
    /// @~spanish
    /// @brief Detiene el sistema de colisiones.
    ~CollisionManager();

    /// @~english
    /// @brief Initializes the collision system.
    /// @~spanish
    /// @brief Inicializa el sistema de colisiones.
    static void Init();

    /// @~english
    /// @brief Access to the single instance of \c CollisionManager .
    /// @return Pointer to the single instance of \c CollisionManager .
    /// @~spanish
    /// @brief Acceso a la instancia única de \c CollisionManager .
    /// @return Puntero a la instancia única de \c CollisionManager .
    static CollisionManager* Instance();

    /// @~english
    /// @brief Stops the collision system.
    /// @~spanish
    /// @brief Detiene el sistema de colisiones.
    static void Shutdown();

    /// @~english
    /// @brief Collision system updating. Updates the colliding state of every registered \c Collider .
    /// @~spanish
    /// @brief Actualización del sistema de colisiones. Actualiza el estado de colisión de cada \c Collider registrado.
    void fixedUpdate();

    /// @~english
    /// @brief Adds a \c Collider to be updated by the collision system.
    /// @param collider Pointer to the \c Collider to register.
    /// @~spanish
    /// @brief Añade un \c Collider a ser actualizado por el sistema de colisiones.
    /// @param collider Puntero al \c Collider a registrar.
    void registerCollider(Collider* collider);

    /// @~english
    /// @brief Removes a \c Collider from being updated by the collision system.
    /// @param collider Pointer to the \c Collider to unregister.
    /// @~spanish
    /// @brief Elimina un \c Collider de ser actualizado por el sistema de colisiones.
    /// @param collider Puntero al \c Collider a desregistrar.
    void unregisterCollider(Collider* collider);
};

#endif //COLLISIONMANAGER_H
