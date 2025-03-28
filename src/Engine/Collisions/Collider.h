#ifndef COLLIDER_H
#define COLLIDER_H

#include <unordered_set>
#include <Core/ComponentTemplate.h>
#include <Utils/Rect.h>
#include <Utils/Vector2.h>

class CollisionManager;
class Transform;

class Collider : public ComponentTemplate<"Collider"> {
private:
    friend CollisionManager;

    Vector2 _pos;
    Vector2 _size;

    Transform const* _transform;

    std::unordered_set<Collider*> _isColliding;
    std::unordered_set<Collider*> _justCollided;
    std::unordered_set<Collider*> _collisionEnded;

public:
    explicit Collider(ComponentData const* data);
    ~Collider() override;
    bool init() override;
    void onEnable() override;
    void onDisable() override;

    /// @~english
    /// @brief Checks whether this \c Collider is colliding with another one.
    /// @param other Pointer to other collider.
    /// @return \c true if they are colliding. \c false otherwise.
    /// @~spanish
    /// @brief Comprueba si este \c Collider está colisionando con otro.
    /// @param other Puntero al otro collider.
    /// @return \c true si están colisionando. \c false si no.
    bool isCollidingWith(Collider* other) const;

    /// @~english
    /// @brief Checks whether this \c Collider started colliding with another one this frame.
    /// @param other Pointer to other collider.
    /// @return \c true if they just started colliding. \c false otherwise.
    /// @~spanish
    /// @brief Comprueba si este \c Collider empezó a colisionar con otro este fotograma.
    /// @param other Puntero al otro collider.
    /// @return \c true si acaban de empezar a colisionar. \c false si no.
    bool justCollidedWith(Collider* other) const;

    /// @~english
    /// @brief Checks whether this \c Collider stopped with another one this frame.
    /// @param other Pointer to other collider.
    /// @return \c true if they just stopped colliding. \c false otherwise.
    /// @~spanish
    /// @brief Comprueba si este \c Collider dejó de colisionar con otro este frame.
    /// @param other Puntero al otro collider.
    /// @return \c true si acaban de dejar de colisionar. \c false si no.
    bool collisionEndedWith(Collider* other) const;

    /// @~english
    /// @brief Access to the dimensions of the collider with its entity's transform applied.
    /// @return Rect within which this collider is contained.
    /// @~spanish
    /// @brief Acceso a las dimensiones del colisionador con la transformación de su entidad aplicada.
    /// @return Rectángulo en el que este colisionador está contenido.
    Rect getRect() const;
};



#endif //COLLIDER_H
