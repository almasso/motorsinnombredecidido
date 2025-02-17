cmake_minimum_required(VERSION 3.16)

# Definimos el nombre del proyecto para Android
# We define the project name for Android
project(main LANGUAGES CXX)

# Incluimos todos los archivos .cpp y .c necesarios para JNI
# We include all necessary .cpp and .c files for JNI

file(GLOB APK_SOURCES
        "${ROOT_DIR}/src/APK/*.cpp"
)

# Definimos la librería para Android
# We define the library for Android
add_library(main SHARED ${APK_SOURCES})

# Incluimos y enlazamos las dependencias necesarias
# We include and link the necessary dependencies
target_link_libraries(main PRIVATE SDL3-shared lua Engine)
target_include_directories(main PRIVATE ${SDL3_INCLUDE_DIRS} ${LUA_INCLUDE_DIRS} ${ROOT_DIR}/src/Engine/ ${ROOT_DIR}/src/APK/)


# Especificamos las propiedades de la librería para Android
# We specify the library properties for Android
set_target_properties(main PROPERTIES
        PREFIX "lib"
)