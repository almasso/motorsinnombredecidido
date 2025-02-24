cmake_minimum_required(VERSION 3.16)
project(DearImGuiCompilation LANGUAGES CXX)

# El repositorio oficial de DearImGui, del que descargaremos el código, no tiene por defecto un CMakeLists para compilar el código como librería, por lo que lo
# crearemos nosotros.
# The official DearImGui repository, from which we will download the code, has no CMakeLists to compile the code as a library, so we'll make it ourselves.

# Incluimos todos los archivos .cpp que estén en la ruta de DearImGui a la compilación
# We include all .cpp files inside DearImGui route to compilation
file(GLOB IMGUI_SOURCES "${IMGUI_INCLUDE_DIRS}/*.cpp")

list(APPEND IMGUI_SOURCES "${IMGUI_BACKENDS_DIRS}/imgui_impl_sdl3.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_BACKENDS_DIRS}/imgui_impl_sdlrenderer3.cpp")

# Creamos la librería estática
# We create the static library
add_library(DearImGui STATIC ${IMGUI_SOURCES})

# Directorios de inclusión
# Include directories
target_link_libraries(DearImGui PRIVATE SDL3-shared)
target_include_directories(DearImGui PUBLIC ${IMGUI_INCLUDE_DIRS})

# Establecemos las propiedades para la salida de la librería
# We set the properties for the library output
set_target_properties(DearImGui PROPERTIES
        PREFIX ""
        OUTPUT_NAME "DearImGui$<$<CONFIG:Debug>:_d>"
)
