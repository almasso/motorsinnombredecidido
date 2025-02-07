cmake_minimum_required(VERSION 3.16)

# Incluimos todos los archivos .cpp que estén en la ruta RAIZ/src/Engine/ a la compilación
# We include all .cpp files inside ROOT/src/Engine/ to compilation
file(GLOB ENGINE_SOURCES "${ROOT_DIR}/src/Engine/*.cpp")

# Definimos la librería
# We define the library
add_library(Engine SHARED ${ENGINE_SOURCES})

# Incluimos y enlazamos SDL3
# We include and link SDL3
target_link_libraries(Engine PRIVATE SDL3-shared lua)
target_include_directories(Engine PRIVATE ${SDL3_INCLUDE_DIRS} ${ROOT_DIR}/src/Engine/ ${LUA_INCLUDE_DIRS})


# Hacemos que la librería dinámica tenga un _d cuando se compile en debug
# We make so that the dynamic library has a _d whenever it's compiled on debug
set_target_properties(Engine PROPERTIES
        PREFIX ""
        OUTPUT_NAME "Engine$<$<CONFIG:Debug>:_d>"
)