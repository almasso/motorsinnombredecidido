#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <Core/ComponentTemplate.h>
#include <Utils/Vector2.h>

class Transform : public ComponentTemplate<Transform, "Transform"> {
    private:
    Vector2 _position;
    Vector2 _scale;
    float _rotation;
    public:
    Transform(ComponentData const*data);
    bool init() override;
    const Vector2& getPosition() const;
    Vector2 getGlobalPosition() const;
    void setPosition(const Vector2& position);
    void setPosition(float x, float y);
    void setPositionX(float x);
    void setPositionY(float y);
    void move(const Vector2& movement);
    void move(float movX, float movY);
    void moveX(float movY);
    void moveY(float movX);
    const Vector2& getScale() const;
    Vector2 getGlobalScale() const;
    void setScale(const Vector2& scale);
    void setScale(float x, float y);
    void setScaleX(float x);
    void setScaleY(float y);
    void doScale(const Vector2& scaleFactor);
    void doScale(float scaleX, float scaleY);
    void doScaleX(float scaleX);
    void doScaleY(float scaleY);
    const float& getRotation() const;
    float getGlobalRotation() const;
    void setRotation(float rotation);
    void rotate(float rotation);
    Transform* getParent() const;

    static void RegisterToLua(sol::state& lua);
};

#endif //TRANSFORM_H
