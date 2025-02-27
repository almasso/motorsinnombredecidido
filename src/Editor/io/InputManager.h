//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_INPUTMANAGER_H
#define MOTORSINNOMBREDECIDIDO_INPUTMANAGER_H

namespace editor::io {
    class InputManager {
    public:
        InputManager() = default;
        bool init();
        void handleInput();
        [[nodiscard]] bool quit() const;
        ~InputManager();
    private:
        bool _quitSignal = false;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_INPUTMANAGER_H
