cmake_minimum_required(VERSION 3.16)
project(LuaCompilation LANGUAGES C)

# El repositorio oficial de Lua, del que descargaremos el código, no tiene por defecto un CMakeLists para compilar el código como librería, por lo que lo
# crearemos nosotros.
# The official Lua repository, from which we will download the code, has no CMakeLists to compile the code as a library, so we'll make it ourselves.

set(LUA_INCLUDE_DIRS "${FETCHCONTENT_BASE_DIR}/lualib-src" "${ROOT_DIR}/src/lua/")

list(GET LUA_INCLUDE_DIRS 0 MAIN_LUA_FILES)

# Incluimos todos los archivos .cpp que estén en la ruta de lua a la compilación
# We include all .cpp files inside lua route to compilation
file(GLOB LUA_SOURCES "${MAIN_LUA_FILES}/*.c")


# Eliminamos lua.c y onelua.c de la compilación, porque contiene un main() que no queremos en una librería
# We remove lua.c and onelua.c from compiling, as it contains a main() function that we don't want in a library
list(REMOVE_ITEM LUA_SOURCES ${MAIN_LUA_FILES}/lua.c)
list(REMOVE_ITEM LUA_SOURCES ${MAIN_LUA_FILES}/onelua.c)

# Creamos la librería estática
# We create the static library
add_library(lua STATIC ${LUA_SOURCES})

# Directorios de inclusión
# Include directories
target_include_directories(lua PUBLIC ${LUA_INCLUDE_DIRS})

# Establecemos las propiedades para la salida de la librería
# We set the properties for the library output
set_target_properties(lua PROPERTIES
        PREFIX ""
        OUTPUT_NAME "lua$<$<CONFIG:Debug>:_d>"
)

