//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_RENDERMANAGER_H
#define MOTORSINNOMBREDECIDIDO_RENDERMANAGER_H

#include <cstdint>
#include <memory>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
struct ImGuiContext;

namespace editor::render {
    /**
     * @~english
     * @brief Class that is responsible for managing the render of the entire \a Editor.
     *
     * @~spanish
     * @brief Clase que se encarga de gestionar el \a renderizado de todo el \a Editor.
     */
    class RenderManager {
    public:
        /**
         * @~english
         * @brief Initializes the render manager.
         *
         * Initializes both SDL and DearImGui rendering functions, being cautious with possible errors and storing the initialization result along the way.
         * If the initialization is successful, the programmer can use the \c render() function; else, the programmer must delete the instance of the
         * render manager using \c delete.
         *
         * @param width Width of the window the render manager creates
         * @param height Height of the window the render manager creates
         *
         * @return \c true if the initialization was sucessful, \c false if the initialization failed.
         *
         * @~spanish Inicializa el gestor de \a renderizado.
         *
         * Inicializa funciones de \a renderizado de SDL y DearImGui, siendo cuidadoso con posibles errores y guardando el resultado de la inicialización
         * mientras esta se hace. Si la inicialización fue correcta, el programador puede usar la función \c render(); de lo contrario, el programador debe
         * borrar la instancia del gestor de \a renderizado utilizando \c delete.
         *
         * @param width Anchura de la ventana que crea el gestor de \a render
         * @param height Altura de la ventana que crea el gestor de \a render
         *
         * @return \c true si la inicialización fue correcta; \c false si la inicialización falló.
         */
        static bool Init(uint32_t width, uint32_t height);

        static RenderManager& GetInstance();

        /**
         * @~english
         * @brief Renders all \a Editor content (windows, modals, etc...).
         *
         * Renders all windows, from the bottom to the top one, making the appropiate calls to API renders from DearImGui and SDL,
         * clears the render buffer and then presents it.
         *
         * @~spanish
         * @brief \a Renderiza todo el contenido del \a Editor (ventanas, ventanas modales, etc...).
         *
         * \a Renderiza todas las ventanas, de abajo hacia arriba, haciendo las llamadas apropiadas a la API de \a renderizado de DearImGui
         * y de SDL, vacía el \a buffer de \a renderizado, y posteriormente lo presenta.
         */
        void render();

        /**
         * @~english
         * @brief Returns the render manager window width.
         *
         * @return Render manager window width.
         *
         * @~spanish
         * @brief Devuelve la anchura de la ventana del gestor de \a render.
         *
         * @return Anchura de la ventana del gestor de \a render.
         */
        [[nodiscard]] inline uint32_t getWidth() const { return _width; }

        /**
         * @~english
         * @brief Returns the render manager's window height.
         *
         * @return Render manager's window height.
         *
         * @~spanish
         * @brief Devuelve la altura de la ventana del gestor de \a render.
         *
         * @return Altura de la ventana del gestor de \a render.
         */
        [[nodiscard]] inline uint32_t getHeight() const {return _height;}

        /**
         * @~english
         * @brief Sets render manager's window width.
         *
         * @param width Width of the window.
         *
         * @~spanish
         * @brief Establece la anchura de la ventana del gestor de \a render.
         *
         * @param width Anchura de la ventana.
         */
        inline void setWidth(uint32_t width) {_width = width;}

        /**
         * @~english
         * @brief Sets render manager's window height.
         *
         * @param height Height of the window.
         *
         * @~spanish
         * @brief Establece la altura de la ventana del gestor de \a render.
         *
         * @param height altura de la ventana.
         */
        inline void setHeight(uint32_t height) {_height = height;}

        template <std::convertible_to<std::string> T>
        void setWindowName(T&& name) {

        }

        RenderManager(const RenderManager &) = delete;
        RenderManager &operator=(const RenderManager &) = delete;

        /**
         * @~english
         * @brief Destructor for the render manager.
         *
         * @~spanish
         * @brief Desturctora del gestor de \a renderizado
         */
        ~RenderManager();
    private:
        /**
         * @~english
         * @brief This enum declares each step at the initialization of the render manager, so the destructor can call closing functions
         * properly without the risk of an execution error or a memory leak.
         *
         * @~spanish
         * @brief Este enumerado declara cada paso de la inicialización del gestor de \a renderizado para que la destructora pueda llamar correctamente
         * a las funciones de cerrado y borrado sin arriesgarnos a un error de ejecución o una fuga de memoria.
         */
        enum class RenderManager_InitializationSteps {
            SDL_INIT_CORRECT = 0b1, ///< @~english SDL was successfully initialized. @~spanish SDL se ha iniciado correctamente.
            SDL_WINDOW_CREATED = 0b10, ///< @~english SDL window was successfully created. @~spanish La ventana de SDL se ha creado correctamente.
            SDL_RENDERER_CREATED = 0b100, ///< @~english SDL renderer was successfully created. @~spanish El \a renderer de SDL se ha creado correctamente.
            IMGUI_CONTEXT_CREATED = 0b1000, ///< @~english DearImGui context was successfully created. @~spanish El contexto de DearImGui se ha creado correctamente.
            IMGUI_SDL3_INIT_CORRECT = 0b10000, ///< @~english DearImGui-SDL3 link was successfully created. @~spanish El enlace entre SDL3 y DearImGui se ha hecho correctamente.
            IMGUI_SDLRENDERER3_INIT_CORRECT = 0b100000 ///< @~english DearImGui-SDL3 Renderer link was successfully created. @~spanish El enlace entre DearImGui y el \a renderer de SDL3 se ha hecho correctamente.
        };

        static std::unique_ptr<RenderManager> _instance;

        /**
         * @~english
         * @brief Pointer to the \c SDL_Window that the render manager is going to use.
         *
         * @~spanish
         * @brief Puntero a la ventana de SDL (\c SDL_Window) que el gestor de \a render va a usar.
         */
        SDL_Window* _window = nullptr;

        /**
         * @~english
         * @brief Pointer to the \c SDL_Renderer that the render manager is going to use.
         *
         * @~spanish
         * @brief Puntero al \a renderer de SDL (\c SDL_Renderer) que el gestor de \a render va a usar.
         */
        SDL_Renderer* _renderer = nullptr;

        /**
         * @~english
         * @brief Pointer to the \c ImGuiContext.
         *
         * @~spanish
         * @brief Puntero al \c ImGuiContext.
         */
        ImGuiContext* _context = nullptr;

        /**
         * @~english
         * @brief Boolean that tells the manager whether the GUI is set to dark mode or not.
         *
         * @~spanish
         * @brief Booleano que le indica al gestor si la GUI está en modo oscuro o no.
         */
        bool _darkModeEnabled = false;

        /**
         * @~english
         * @brief Integer that is going to store the steps on the initialization of the manager to check whether
         * all subsystems have been initialized correctly.
         *
         * @~spanish
         * @brief Entero que va a almacenar todos los pasos en la inicialización del gestor para comprobar si
         * se han iniciado correctamente todos los subsistemas.
         */
        uint8_t _initializationSteps = 0;

        /**
         * @~english
         * @brief Width of the window created by the render manager.
         *
         * @~spanish
         * @brief Anchura de la ventana creada por el gestor de \a render.
         */
        uint32_t _width;

        /**
         * @~english
         * @brief Height of the window created by the render manager.
         *
         * @~spanish
         * @brief Altura de la ventana creada por el gestor de \a render.
         */
        uint32_t _height;

        /**
         * @~english
         * @brief Default constructor for the render manager.
         *
         * @~spanish
         * @brief Constructora por defecto para el gestor de \a renderizado.
         */
        RenderManager() = default;

        bool init(uint32_t width, uint32_t height);

        /**
         * @~english
         * @brief Inits all SDL subsystems (SDL video, SDL window and SDL renderer).
         *
         * @return \c true if the initialization was successful, \c false if the initialization failed.
         *
         * @~spanish
         * @brief Inicializa todos los subsistemas de SDL (SDL video, ventana de SDL y \a renderer de SDL).
         *
         * @return \c true si la inicialización fue correcta, \c false si la inicialización falló.
         */
        bool initSDL();

        /**
         * @~english
         * @brief Inits all DearImGui subsystems (ImGui context, ImGui link with SDL and ImGui link with SDL renderer).
         *
         * @return \c true if the initialization was successful, \c false if the initialization failed.
         *
         * @~spanish
         * @brief Inicializa todos los subsistemas de DearImGui (Contexto de ImGUI, enlace de ImGui con SDL y enlace de ImGUI con el \a renderer de SDL).
         *
         * @return \c true si la inicialización fue correcta, \c false si la inicialización falló.
         */
        bool initDearImGui();
    };
}



#endif //MOTORSINNOMBREDECIDIDO_RENDERMANAGER_H
