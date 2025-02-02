cmake_minimum_required(VERSION 3.16)

# Incluimos todos los archivos .cpp que estén en la ruta RAIZ/src/Engine/ a la compilación
# We include all .cpp files inside ROOT/src/Engine/ to compilation
file(GLOB ENGINE_SOURCES "${ROOT_DIR}/src/Engine/*.cpp")

# Definimos la librería
# We define the library
add_library(Engine SHARED ${ENGINE_SOURCES})

# Incluimos y enlazamos SDL3
# We include and link SDL3
target_link_libraries(Engine PRIVATE SDL3::SDL3 lua::lua)
target_include_directories(Engine PRIVATE ${SDL3_INCLUDE_DIRS} ${ROOT_DIR}/src/Engine/ ${LUA_INCLUDE_DIRS})