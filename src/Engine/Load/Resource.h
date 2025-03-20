#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

/// @~english
/// @brief Base interface to implement different types of resources.
/// @~english
/// @brief Interfaz base para implementar diferentes tipos de recursos.
class Resource {
protected:
    int _size;
    std::string _path;

public:
    /// @~english
    /// @brief Creates the \c Resource with an assigned file path.
    /// @param path Relative path from the executable to the resource file.
    /// @~spanish
    /// @brief Crea el \c Resource con una ruta de archivo asignada.
    /// @param path Ruta relativa desde el ejecutable hasta el archivo de recurso.
    explicit Resource(std::string const& path);

    /// @~english
    /// @brief Default destructor.
    /// @~spanish
    /// @brief Destructor por defecto.
    virtual ~Resource();

    /// @~english
    /// @brief Read the file in the specified path and store it.
    /// @return \c true on successful loading. \c false on failure.
    /// @~spanish
    /// @brief Lee le archivo en la ruta especificada y lo almacena.
    /// @return \c true si carga exitosamente. \c false si falla.
    virtual bool load() = 0;

    /// @~english
    /// @brief Erase the specific memory of the \c Resource stored.
    /// @~spanish
    /// @brief Elimina la memoria específica del \c Resource almacenado.
    virtual void unload() = 0;

    /// @~english
    /// @brief Gets the size of the stored \c Resource memory.
    /// @return Size in bytes of the stored memory. \c -1 if the \c Resource isn't loaded.
    /// @~spanish
    /// @brief Accede al tamaño de la memoria de \c Resource almacenada.
    /// @return Tamaño en bytes de la memoria almacenada. \c -1 si el \c Resource no está cargado.
    inline int getSize() const { return _size; }
};


#endif //RESOURCE_H
