//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_EDITOR_H
#define MOTORSINNOMBREDECIDIDO_EDITOR_H

#include <memory>

namespace editor {

    namespace render {
        class RenderManager;
    }

    namespace io {
        class InputManager;
    }

    /**
     * @~english
     * @brief Main class and entry point for the \a Editor
     *
     * @~spanish
     * @brief Clase principal y punto de entrada del \a Editor
     */
    class Editor {
    public:
        /**
         * @~english
         * @brief Initializes the \a Editor's singleton, as well as all dependencies.
         *
         * This function serves as an entry point for the \a Editor, and needs to be called in the \c main function to initialize
         * all resources used by the \a Editor (external libraries and our own classes).
         *
         * @important Programmer needs to make sure that \c Init <b> is just called once </b> in the execution of the program, or else
         * an \c assert error will be thrown.
         *
         * @return \c true for a successful initialization of the singleton, \c false if the initialization failed.
         *
         * @remark If the initialization failed, the programmer does not need to do anything. All memory is safely deleted and
         * no memory leaks should remain.
         *
         * @~spanish
         * @brief Inicializa el \a singleton del \a Editor, incluyendo todas las dependencias usadas.
         *
         * Esta función sirve como un punto de entrada al \a Editor, y necesita ser llamada en la función \c main del programa para
         * inicializar todos los recursos utilizados por el \a Editor (librerías externas y nuestras propias clases).
         *
         * @important El programador se tiene que asegurar de que \c Init <b> se llama una única vez </b> en la ejecución del programa, o
         * entonces se lanzará un error de \c assert.
         *
         * @return \c true si la inicialización se ha hecho correctamente, \c false si la inicialización ha fallado.
         *
         * @remark Si la inicialización ha fallado, el programador no necesita hacer nada. Toda la memoria se borra de manera segura
         * y no debería dejar ninguna fuga de memoria.
         */
        static bool Init();

        /**
         * @~english
         * @brief Returns a reference to the singleton instance of the \a Editor.
         *
         * @important Programmer needs to make sure that \c Init is called and the initialization was successful before calling
         * \c GetInstance, as \c GetInstance will not initialize the \a Editor's singleton and an \c assert error will be thrown.
         *
         * @remark As a safety measure, this method will not return a pointer to the singleton instance, but a reference. The programmer
         * is still able to store the memory address of the singleton instance, but <b>it is not advisable</b> as memory is already managed
         * by the class and an unknown non-treated pointer can lead to memory leaks.
         *
         * @return Reference to the singleton instance of the \a Editor.
         *
         * @~spanish
         * @brief Devuelve una referencia a la instancia \a singleton del \a Editor.
         *
         * @important El programador tiene que asegurarse de que \c Init se ha llamado y de que la inicialización ha sido correcta
         * antes de llamar a \c GetInstance, puesto que \c GetInstance no inicializará la instancia del \a singleton del \a Editor y
         * se lanzará un error de \c assert.
         *
         * @remark Como medida de seguridad, esta función no devolverá un puntero a la instancia \a singleton, sino que devolverá una referencia.
         * El programador puede guardarse la dirección de memoria de la instancia \a singleton, <b>pero es totalmente desaconsejable</b>,
         * ya que la clase ya tiene su propio manejo de memoria y un puntero desconocido que no tenga un tratamiento correcto puede llevar a
         * fugas de memoria.
         *
         * @return Referencia a la instancia \a singleton del \a Editor.
         */
        static Editor &GetInstance();

        /**
         * @~english
         * @brief Runs the main loop for the \a Editor.
         *
         * The main loop for the \a Editor calls rendering functions as well as input handling functions to render all windows
         * that appear on the screen simultaneously and to handle all input that occurs on each one of the screens. The programmer
         * <b>does not need to run this function on a loop</b>, as the loop is already inside the function, and only needs to call it
         * once (for example, on the \c main function after initializing the singleton instance). Once all windows are closed and no
         * processing is necessary (thus, ending the program execution), the loop will safely exit.
         *
         * @~spanish
         * @brief Ejecuta el bucle principal del \a Editor.
         *
         * El bucle principal del \a Editor llama a funciones de \a renderizado, así como funciones de control de la entrada del usuario para, \a renderizar
         * todas las ventanas del \a Editor que aparezcan en pantalla al mismo tiempo y para controlar toda la entrada que el usuario ejerza sobre cualquiera
         * de las ventanas. El programador <b>no necesita ejecutar esta función dentro de un bucle</b> ya que el bucle es interno a la función y solo se necesita
         * llamar una única vez (por ejemplo, en la función \c main después de inicializar la instancia \a singleton). Una vez que todas las ventanas se hayan
         * cerrado y no se necesite procesar nada más (acabándose la ejecución del programa), el bucle terminará de manera segura.
         */
        void mainLoop();

        /**
         * @~english
         * As we don't want the programmer to store the reference to the \a Editor in a variable somewhere, we forbid them to use the copy constructor.
         *
         * @~spanish
         * Como no queremos que el programador se guarde la referencia al \a Editor en una variable, le prohibimos usar el constructor por copia.
         */
        Editor(const Editor &) = delete;

        /**
         * @~english
         * As we don't want the programmer to store the reference to the \a Editor in a variable somewhere, we forbid them to use the assign operator.
         *
         * @~spanish
         * Como no queremos que el programador se guarde la referencia al \a Editor en una variable, le prohibimos usar el operador de asignación.
         */
        Editor &operator=(const Editor &) = delete;

        /**
         * @~english
         * @brief \a Editor's destructor.
         *
         * @warning Programmer should never call \c delete on the singleton instance, as instance deletion is automatically handled by this class.
         *
         * @~spanish
         * @brief Destructora del \a Editor
         *
         * @warning El programador no debe nunca llamar a \c delete en la instancia \a singleton, ya que la clase se encarga automáticamente del borrado de
         * esta.
         */
        ~Editor();

    private:

        /**
         * @~english
         * @brief Unique pointer that stores the singleton instance of the \a Editor.
         *
         * @~spanish
         * @brief Puntero único (\a unique \a pointer) que almacena la instancia \a singleton del \a Editor.
         */
        static std::unique_ptr<Editor> _instance;

        /**
         * @~english
         * @brief Boolean that stores whether the main loop should run or not.
         *
         * @~spanish
         * @brief Booleano que almacena si el bucle principal debe o no ejecutarse.
         */
        bool _running = true;

        /**
         * @~english
         * @brief Default constructor for the \a Editor class. Private as we don't want the programmer to create an instance without it being a singleton.
         *
         * @~spanish
         * @brief Constructora por defecto de la clase \a Editor. Es privada ya que no queremos que el programador cree una instancia sin esta ser \a singleton.
         */
        Editor() = default;

        /**
         * @~english
         * @brief Initializes the singleton's instance of the \a Editor.
         *
         * @sa Init()
         *
         * @return \c true for a successful initialization of the singleton, \c false if the initialization failed.
         *
         * @~spanish
         * @brief Inicializa la instancia del \a singleton del \a Editor.
         *
         * @sa Init()
         *
         * @return \c true si la inicialización se ha hecho correctamente, \c false si la inicialización ha fallado.
         */
        bool init();
    };
}

#endif //MOTORSINNOMBREDECIDIDO_EDITOR_H