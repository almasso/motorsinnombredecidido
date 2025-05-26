cmake_minimum_required(VERSION 3.16)

# Incluimos todos los archivos .cpp que estén en la ruta RAIZ/src/Executable/ a la compilación
# We include all .cpp files inside ROOT/src/Executable/ to compilation
file(GLOB EXECUTABLE_SOURCES "${ROOT_DIR}/src/Executable/*.cpp")

# Definimos el ejecutable
# We define the executable
add_executable(Executable ${EXECUTABLE_SOURCES})

# Vinculamos el proyecto Engine al Executable
# We link Engine project to the Executable project
target_link_libraries(Executable PRIVATE Engine)

# Incluimos y enlazamos SDL3
# We include and link SDL3
target_link_libraries(Executable PRIVATE SDL3-shared)
target_include_directories(Executable PRIVATE ${SDL3_INCLUDE_DIRS} ${ROOT_DIR}/src/Executable/ ${ROOT_DIR}/src/Engine/)

# Hacemos que el ejecutable tenga un _d cuando se compile en debug
# We make so that the executable has a _d whenever it's compiled on debug
set_target_properties(Executable PROPERTIES
        OUTPUT_NAME "Executable$<$<CONFIG:Debug>:${CMAKE_DEBUG_POSTFIX}>"
)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(Executable PROPERTIES WIN32_EXECUTABLE TRUE)
endif()