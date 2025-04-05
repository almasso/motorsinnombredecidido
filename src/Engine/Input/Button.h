#ifndef BUTTON_H
#define BUTTON_H
#include <string>
#include <Core/ComponentTemplate.h>
#include <sol/protected_function.hpp>
#include <sol/table_core.hpp>
#include <Utils/Vector2.h>


class Camera;
class Transform;

class ComponentClass(Button) {
    private:
        sol::function _callback;
        sol::table _params;
        Vector2 _size;
        Transform* _transform;
        Camera* _camera;

    public:
    Button(ComponentData const* data);
    bool init() override;
    bool update() override;
    void setCallback(const sol::function &callback);
    static void RegisterToLua(sol::state &lua);
};

#endif //BUTTON_H
