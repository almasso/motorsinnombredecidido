cmake_minimum_required(VERSION 3.16)

project(motorsinnombredecidido LANGUAGES C CXX)

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
set(DEPENDENCIES_DIR "${ROOT_DIR}/dependencies/${CMAKE_SYSTEM_NAME}")

# Le decimos a FetchContent a qué carpeta descargar los archivos necesarios
# We tell FetchContent which folder where's going to download the dependencies
set(FETCHCONTENT_BASE_DIR ${DEPENDENCIES_DIR}/${CMAKE_BUILD_TYPE})

# Directorios de salida / Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/libs/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
if(ANDROID)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${ANDROID_ABI})
endif()
if(NOT ANDROID AND ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX "_d")
endif()

# Obtenemos SDL 3.2.2 (última versión estable release a la fecha)
# We fetch SDL 3.2.2 (last stable release version to date)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.8
)
FetchContent_MakeAvailable(SDL3)
FetchContent_GetProperties(SDL3)
set(SDL3_INCLUDE_DIRS ${SDL3_SOURCE_DIR}/include)

# Obtenemos también todas las librerías extra de SDL, en este caso, SDL_ttf y SDL_image
# We also fetch all extra SDL libraries, in this case, SDL_ttf and SDL_image

FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG VER-2-13-2
)
set(DISABLE_FORCE_DEBUG_POSTFIX ON)
FetchContent_MakeAvailable(freetype)
set(FREETYPE_INCLUDE_DIR "${freetype_SOURCE_DIR}/include")
set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FREETYPE_LIBRARY_NAME "libfreetype${CMAKE_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
    set(FREETYPE_LIBRARY_NAME "libfreetype${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()
set(FREETYPE_LIBRARY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FREETYPE_LIBRARY_NAME}")

FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-3.2.0
)
FetchContent_MakeAvailable(SDL3_ttf)
set(SDL3_TTF_INCLUDE_DIRS ${SDL3_TTF_SOURCE_DIR}/include)
FetchContent_GetProperties(SDL3_ttf)

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.2.4
)
FetchContent_MakeAvailable(SDL3_image)
FetchContent_GetProperties(SDL3_image)
set(SDL3_IMAGE_INCLUDE_DIRS ${SDL3_IMAGE_SOURCE_DIR}/include)


# Hacemos lo mismo con LUA v5.4.7
# We do the same with LUA v5.4.7
FetchContent_Declare(
        lualib
        GIT_REPOSITORY https://github.com/lua/lua.git
        GIT_TAG v5.4.7
)
FetchContent_MakeAvailable(lualib)

# Y con sol2, una librería de bindings de lua
# And with sol2, a binding library for lua
FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol2)
set(SOL2_INCLUDE_DIRS ${sol2_SOURCE_DIR}/include)

# Y con DearImGUI
# And with DearImGUI
if(NOT ANDROID)
    FetchContent_Declare(
            DearImGUI
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG docking
    )
    FetchContent_MakeAvailable(DearImGUI)
    set(IMGUI_INCLUDE_DIRS ${FETCHCONTENT_BASE_DIR}/dearimgui-src)
    set(IMGUI_BACKENDS_DIRS ${FETCHCONTENT_BASE_DIR}/dearimgui-src/backends)
endif()

# Añadimos los subdirectorios de cada uno de nuestros proyectos
# We add the subdirectories for each one of our projects
add_subdirectory(${ROOT_DIR}/projects/lua)
add_subdirectory(${ROOT_DIR}/projects/Engine)
if(NOT ANDROID)
add_subdirectory(${ROOT_DIR}/projects/DearImGui)
add_subdirectory(${ROOT_DIR}/projects/Editor)
add_subdirectory(${ROOT_DIR}/projects/Executable)
endif()
if(ANDROID)
    add_subdirectory(${ROOT_DIR}/projects/APK/app)
endif()


# Búsqueda de libgcc_s_seh-1.dll y de libstdc++-6.dll en Windows, Mac y Linux
# Search of libgcc_s_seh-1.dll and libstdc++-6.dll on Windows, Mac and Linux
if(WIN32)
    # Windows
    find_path(MINGW_BIN_DIR
            NAMES
            libgcc_s_seh-1.dll
            libstdc++-6.dll
            libatomic-1.dll
            libgomp-1.dll
            libssp-0.dll
            libwinpthread-1.dll
            PATHS
            "C:/MinGW/bin"
            "C:/Jetbrains/CLion/bin/mingw/bin"
            "E:/Jetbrains/CLion/bin/mingw/bin"
            "$ENV{PATH}"
    )

    if(MINGW_BIN_DIR)
        set(REQUIRED_DLLS
                "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
                "${MINGW_BIN_DIR}/libstdc++-6.dll"
                "${MINGW_BIN_DIR}/libatomic-1.dll"
                "${MINGW_BIN_DIR}/libgomp-1.dll"
                "${MINGW_BIN_DIR}/libssp-0.dll"
                "${MINGW_BIN_DIR}/libwinpthread-1.dll"
        )

        file(COPY ${REQUIRED_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    else()
        message(WARNING "[EN] Could not find MinGW bin directory. Required DLLs will not be copied.
                        [ES] No se ha podido encontrar el directorio bin de MinGW. Las DLL requeridas no se copiarán.")
    endif()
elseif(APPLE)
    # Mac
    set(REQUIRED_LIBS
            libgcc_s.1.dylib
            libstdc++.6.dylib
            libatomic.1.dylib
            libgomp.1.dylib
            libssp.0.dylib
            libpthread.dylib
    )

    foreach(LIB ${REQUIRED_LIBS})
        find_library(LIB_PATH ${LIB}
                PATHS
                /usr/lib
                /usr/local/lib
                /opt/homebrew/lib
        )

        if(LIB_PATH)
            file(COPY ${LIB} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        else()
            message(WARNING "[EN] Could not find ${LIB}. Required library will not be copied.
                            [ES] No se ha podido encontrar ${LIB}. Las librerías requeridas no se copiarán.")
        endif()
    endforeach()
elseif(UNIX)
    # Linux
    set(REQUIRED_LIBS
            libgcc_s.so.1
            libstdc++.so.6
            libatomic.so.1
            libgomp.so.1
            libssp.so.0
            libpthread.so.0
    )

    foreach(LIB ${REQUIRED_LIBS})
        find_library(LIB_PATH ${LIB}
                PATHS
                /usr/lib
                /usr/lib/x86_64-linux-gnu
                /lib/x86_64-linux-gnu
        )

        if(LIB_PATH)
            file(COPY ${LIB} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        else()
            message(WARNING "[EN] Could not find ${LIB}. Required library will not be copied.
                            [ES] No se ha podido encontrar ${LIB}. Las librerías requeridas no se copiarán.")
        endif()
    endforeach()
endif()
