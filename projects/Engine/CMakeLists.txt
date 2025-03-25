cmake_minimum_required(VERSION 3.16)

# Incluimos todos los archivos .cpp que estén en la ruta RAIZ/src/Engine/ a la compilación
# We include all .cpp files inside ROOT/src/Engine/ to compilation
file(GLOB_RECURSE ENGINE_SOURCES "${ROOT_DIR}/src/Engine/*.cpp")

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build Engine as a shared library | Compilar Engine como una librería dinámica" FORCE)

# Definimos la librería
# We define the library
add_library(Engine SHARED ${ENGINE_SOURCES})

add_dependencies(Engine freetype)

# Incluimos y enlazamos SDL3
# We include and link SDL3
target_link_libraries(Engine PRIVATE SDL3-shared lua sol2::sol2 SDL3_ttf::SDL3_ttf-shared SDL3_image::SDL3_image-shared)
target_include_directories(Engine PRIVATE ${SDL3_INCLUDE_DIRS} ${ROOT_DIR}/src/Engine/ ${LUA_INCLUDE_DIRS} ${SDL3_TTF_INCLUDE_DIRS} ${SDL3_IMAGE_INCLUDE_DIRS} ${SOL2_INCLUDE_DIRS})
