cmake_minimum_required(VERSION 3.16)

project(motorsinnombredecidido LANGUAGES CXX)

# Establecemos el tipo de compilación por defecto a Release
# We set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# Añadimos Debug y Release a los tipos de compilación posibles
# We add Debug and Release to the possible compilation types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-static -static-libgcc -static-libstdc++)
add_link_options(-static -static-libgcc -static-libstdc++)

# Habilitamos FetchContent para la descarga de las dependencias
# We enable FetchContent for dependencies download
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# Directorio raíz / Root directory
# Nos guardamos en una variable la ruta de la raíz del proyecto
# We store in a variable the route from project's root
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Establecemos el directorio de las dependencias de nuestro proyecto
# We set the directory for our project dependencies
set(DEPENDENCIES_DIR "${ROOT_DIR}/dependencies")

# Le decimos a FetchContent a qué carpeta descargar los archivos necesarios
# We tell FetchContent which folder where's going to download the dependencies
set(FETCHCONTENT_BASE_DIR ${DEPENDENCIES_DIR}/${CMAKE_BUILD_TYPE})

# Directorios de salida / Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/libs/${CMAKE_BUILD_TYPE})

# Obtenemos SDL 3.2.2 (última versión estable release a la fecha)
# We fetch SDL 3.2.2 (last stable release version to date)
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.2
)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build SDL3 as a shared library")
FetchContent_MakeAvailable(SDL3)

FetchContent_GetProperties(SDL3)
set(SDL3_INCLUDE_DIRS ${SDL3_SOURCE_DIR}/include)

# Hacemos lo mismo con LUA v5.4.7
# We do the same with LUA v5.4.7
FetchContent_Declare(
        lualib
        GIT_REPOSITORY https://github.com/lua/lua.git
        GIT_TAG v5.4.7
)
FetchContent_MakeAvailable(lualib)

# Y con DearImGUI
# And with DearImGUI
FetchContent_Declare(
        DearImGUI
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.8
)
FetchContent_MakeAvailable(DearImGUI)

set(IMGUI_INCLUDE_DIRS ${FETCHCONTENT_BASE_DIR}/dearimgui-src)
set(IMGUI_BACKENDS_DIRS ${FETCHCONTENT_BASE_DIR}/dearimgui-src/backends)

# Añadimos los subdirectorios de cada uno de nuestros proyectos
# We add the subdirectories for each one of our projects
add_subdirectory(${ROOT_DIR}/projects/lua)
add_subdirectory(${ROOT_DIR}/projects/Engine)
add_subdirectory(${ROOT_DIR}/projects/Editor)
add_subdirectory(${ROOT_DIR}/projects/Executable)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set_target_properties(SDL3-shared PROPERTIES OUTPUT_NAME "SDL3_d")
endif()