//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_EDITOR_H
#define MOTORSINNOMBREDECIDIDO_EDITOR_H

/**
 * @~english
 * @brief Main class and entry point for the Editor
 *
 * @~spanish
 * @brief Clase principal y punto de entrada del Editor
 */
class Editor {
public:
    /**
     * @~english
     * @brief Initializes the singleton instance of the Editor, as well as all dependences.
     *
     * This function serves as an entry point for the Editor, and needs to be called in the main function to initialize
     * all resources used by the Editor (external libraries and our own classes).
     *
     *
     * @~spanish
     * @brief Inicializa la instancia singleton del Editor, incluyendo todas las dependencias usadas.
     *
     */
    static void Init();

    static Editor* GetInstance();

    ~Editor();
private:

    static Editor* _instance;
    
    Editor() = default;

    bool init();
};

#endif //MOTORSINNOMBREDECIDIDO_EDITOR_H
