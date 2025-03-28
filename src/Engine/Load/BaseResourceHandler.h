#ifndef BASERESOURCEHANDLER_H
#define BASERESOURCEHANDLER_H

class ResourceMemoryManager;

/// @~english
/// @brief Super class for initialization and shutdown of every \c ResourceHandler from the \c ResourceManager.
/// @~spanish
/// @brief Súperclase para la inicialización y apagado de cada \c ResourceHandler desde el \c ResourceManager.
class BaseResourceHandler {
protected:
    ResourceMemoryManager* _resourceMemoryManager;
public:
    /// @~english
    /// @brief Creates an uninitialized \c BaseResourceHandler.
    /// @~spanish
    /// @brief Crea un \c BaseResourceHandler sin inicializar.
    BaseResourceHandler();

    /// @~english
    /// @brief Destroys the \c BaseResourceHandler.
    /// @~spanish
    /// @brief Destruye el \c BaseResourceHandler.
    virtual ~BaseResourceHandler();

    /// @~english
    /// @brief Initializes the \c BaseResourceHandler with a given \c ResourceMemoryManager to handle the loading of the handler's resources.
    /// @param memoryManager Pointer to an instance of a \c ResourceMemoryManager.
    /// @~spanish
    /// @brief Initializes the \c BaseResourceHandler with a given \c ResourceMemoryManager to handle the loading of the handler's resources.
    /// @param memoryManager Pointer to an instance of a \c ResourceMemoryManager.
    void init(ResourceMemoryManager* memoryManager);

    /// @~english
    /// @brief Stops the \c BaseResourceHandler 's work. It becomes invalidated for later use.
    /// @~spanish
    /// @brief Detiene el funcionamiento del \c BaseResourceHandler. Queda invalidado para futuro uso.
    void shutdown();
};



#endif //BASERESOURCEHANDLER_H
