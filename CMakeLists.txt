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
find_package(SndFile REQUIRED)
find_package(soil2 CONFIG REQUIRED)
find_package(assimp REQUIRED)

add_executable(VGI-ABP 
# Ficheros principales
        src/main.cpp
        src/Game.cpp
        src/Game.h

# Ficheros de funcionalidades extra
        src/Encabezados/stdafx.h
        src/Encabezados/constants.h

# Ficheros de funcionalidades del juego
        src/General/Camera.h
        src/General/Object.cpp
        src/General/Object.h
        src/General/Player.cpp
        src/General/Player.h

# Parte Graficos
        src/Graficos/Renderer.h 
        src/Graficos/Renderer.cpp
        src/Graficos/Shader.h
        src/Graficos/Skybox.h
        src/Graficos/Skybox.cpp

# Ficheros de Objeto
        src/Graficos/Modelo/Model.h
        src/Graficos/Modelo/Mesh.h

# Ficheros para la iluminacion
        src/Graficos/Luces/Light.cpp
        src/Graficos/Luces/Light.h

# Ficheros para el sonido
        src/Sound/MusicBuffer.cpp
        src/Sound/MusicBuffer.h
        src/Sound/SoundDevice.cpp
        src/Sound/SoundDevice.h
        src/Sound/SoundSource.cpp
        src/Sound/SoundSource.h

# Ficheros Unknown
        src/Unknown/Window.cpp
        src/Unknown/Window.h
        src/Unknown/PhysicsEngine.cpp
        src/Unknown/PhysicsEngine.h
        src/Unknown/PlayerVehicle.h
        src/Unknown/PlayerVehicle.cpp
        src/Unknown/GroundMesh.h
        src/Unknown/GLDebugDrawer.h
        src/Unknown/GLDebugDrawer.cpp)

# target_link_libraries(VGI-ABP PRIVATE glfw "${ASSIMP_INCLUDE_DIRS}/assimp" "${BULLET_LIBRARIES}" GLEW::glew OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenGL::GL)
target_link_libraries(VGI-ABP PRIVATE GLEW::GLEW)
target_link_libraries(VGI-ABP PRIVATE glfw)
target_link_libraries(VGI-ABP PRIVATE "${BULLET_LIBRARIES}")
target_link_libraries(VGI-ABP PRIVATE SndFile::sndfile)
target_link_libraries(VGI-ABP PRIVATE soil2)
target_link_libraries(VGI-ABP PRIVATE ${ASSIMP_LIBRARIES})
