#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

class InputManager {
    public:
    InputManager();
    bool init();
    bool checkExit();
    void shutdown() const;

};

#endif //INPUTMANAGER_H
