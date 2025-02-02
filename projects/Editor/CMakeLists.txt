cmake_minimum_required(VERSION 3.16)

# Incluimos todos los archivos .cpp que estén en la ruta RAIZ/src/Editor/ a la compilación
# We include all .cpp files inside ROOT/src/Editor/ to compilation
file(GLOB EDITOR_SOURCES "${ROOT_DIR}/src/Editor/*.cpp")

# Definimos el ejecutable
# We define the executable
add_executable(Editor ${EDITOR_SOURCES})

# Vinculamos el proyecto Engine al Editor
# We link Engine project to the Editor project
target_link_libraries(Editor PRIVATE Engine)

# Incluimos y enlazamos SDL3 para utilizar con DearImGui
# We include and link SDL3 to use with DearImGui
target_link_libraries(Editor PRIVATE SDL3::SDL3 lua::lua)
target_include_directories(Editor PRIVATE ${SDL3_INCLUDE_DIRS} ${ROOT_DIR}/src/Editor/ ${ROOT_DIR}/src/Engine/)