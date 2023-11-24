cmake_minimum_required(VERSION 3.26)
project(VGI-ABP)

set(CMAKE_CXX_STANDARD 14)
IF (WIN32)
    # Add the path to the vcpkg toolchain file
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
ENDIF()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(soil2 CONFIG REQUIRED)
find_package(assimp REQUIRED)

add_executable(VGI-ABP 
# Ficheros principales
        src/main.cpp
# Ficheros de funcionalidades extra
        "src/include/stdafx.h"
        "src/include/stb_image.h"
        
# Fiechros de funcionalidades del juego
        src/General/Camera.h
        src/General/Camera.cpp
        src/General/Skybox.h
        src/General/Skybox.cpp
        src/General/Shader.h
        src/General/Shader.cpp

# Ficheros de Objecto
        src/Modelo/Model.h
        src/Modelo/Model.cpp
        src/Modelo/Mesh.h
        src/Modelo/Mesh.cpp
        src/Modelo/Texture.h
        src/Modelo/Texture.cpp 

# Ficheros para la iluminacion
        src/Luces/DirectionalLight.cpp
        src/Luces/DirectionalLight.h
        src/Luces/Light.cpp
        src/Luces/Light.h
        src/Luces/PointLight.cpp
        src/Luces/PointLight.h
        src/Luces/SpotLight.cpp
        src/Luces/SpotLight.h  "src/Renderer.h" "src/Renderer.cpp" "src/globals.h")


# target_link_libraries(VGI-ABP PRIVATE glfw "${ASSIMP_INCLUDE_DIRS}/assimp" "${BULLET_LIBRARIES}" GLEW::glew OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenGL::GL)
target_link_libraries(VGI-ABP PRIVATE GLEW::GLEW)
target_link_libraries(VGI-ABP PRIVATE glfw)
target_link_libraries(VGI-ABP PRIVATE "${BULLET_LIBRARIES}")
target_link_libraries(VGI-ABP PRIVATE soil2)
target_link_libraries(VGI-ABP PRIVATE ${ASSIMP_LIBRARIES})