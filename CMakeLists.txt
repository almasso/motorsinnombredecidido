cmake_minimum_required(VERSION 3.16)

project(motorsinnombredecidido LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitamos FetchContent para la descarga de las dependencias
# We enable FetchContent for dependencies download
include(FetchContent)

# Directorio raíz / Root directory
# Nos guardamos en una variable la ruta de la raíz del proyecto
# We store in a variable the route from project's root
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Establecemos el directorio de las dependencias de nuestro proyecto
# We set the directory for our project dependencies
set(DEPENDENCIES_DIR "${ROOT_DIR}/dependencies")

# Le decimos a FetchContent a qué carpeta descargar los archivos necesarios
# We tell FetchContent which folder where's going to download the dependencies
set(FETCHCONTENT_BASE_DIR ${DEPENDENCIES_DIR})

# Directorios de salida / Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/libs/${CMAKE_BUILD_TYPE})

# Obtenemos SDL 3.2.2 (última versión estable release a la fecha)
# We fetch SDL 3.2.2 (last stable release version to date)
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.2
)
FetchContent_MakeAvailable(SDL3)

set(SDL3_INCLUDE_DIRS ${SDL3_SOURCE_DIR}/include)

# Hacemos lo mismo con LUA v5.4.7
# We do the same with LUA v5.4.7
FetchContent_Declare(
        lua
        GIT_REPOSITORY https://github.com/marovira/lua.git
        GIT_TAG 5.4.7
)
FetchContent_MakeAvailable(lua)

set(LUA_INCLUDE_DIRS ${lua_SOURCE_DIR}/src)

# Añadimos los subdirectorios de cada uno de nuestros proyectos
# We add the subdirectories for each one of our projects
add_subdirectory(projects/Engine)
add_subdirectory(projects/Editor)
add_subdirectory(projects/Executable)

# Vamos a mover las librerías dinámicas de SDL y LUA hasta bin/${CMAKE_BUILD_TYPE}, donde se generará nuestro ejecutable
# We are moving SDL and LUA dynamic libraries into bin/${CMAKE_BUILD_TYPE}, where our executable will generate
if(TARGET SDL3)
    get_target_property(SDL3_BINARY_DIR SDL3 BINARY_DIR)

    if(WIN32)
        set(DYLIB_EXTENSION ".dll")
    elseif(APPLE)
        set(DYLIB_EXTENSION ".dylib")
    else()
        set(DYLIB_EXTENSION ".so")
    endif()

    add_custom_command(TARGET SDL3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_BINARY_DIR}/lib/${DYLIB_EXTENSION}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
            COMMENT "SDL3${DYLIB_EXTENSION} copiado a/copied to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endif()

if(TARGET lua)
    get_target_property(LUA_BINARY_DIR lua BINARY_DIR)

    if(WIN32)
        set(DYLIB_EXTENSION ".dll")
    elseif(APPLE)
        set(DYLIB_EXTENSION ".dylib")
    else()
        set(DYLIB_EXTENSION ".so")
    endif()

    add_custom_command(TARGET lua POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LUA_BINARY_DIR}/lib/${DYLIB_EXTENSION}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
            COMMENT "LUA${DYLIB_EXTENSION} copiado a/copied to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endif()