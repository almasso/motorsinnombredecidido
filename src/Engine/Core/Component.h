#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
private:
public:
    virtual ~Component() = default;

    Component();
    bool update();
    bool fixedUpdate();
};

#endif //COMPONENT_H
