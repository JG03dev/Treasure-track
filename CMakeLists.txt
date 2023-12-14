cmake_minimum_required(VERSION 3.24.3)
project(VGI-ABP)

# VCPKG TOOLCHAIN
set(CMAKE_CXX_STANDARD 14)
IF (WIN32)
    # Add the path to the vcpkg toolchain file
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
ENDIF()

# TEST LIBRARY
include(GoogleTest)
enable_testing()
find_package(GTest CONFIG REQUIRED)

# FIND ALL PACKAGES
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED PATHS "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/glew-x.y.z/include")
find_package(glm CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SndFile REQUIRED)
find_package(soil2 CONFIG REQUIRED)
find_package(assimp REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

set(GAME_SOURCES
        src/Game.cpp
        src/Game.h
        # Ficheros de funcionalidades extra
        src/Encabezados/stdafx.h
        src/Encabezados/stb_image.h
        src/Encabezados/json.hpp
        
        # Fiechros de funcionalidades del juego
        src/General/Camera.h
        src/General/Camera.cpp
        src/General/Object.cpp
        src/General/Object.h
        src/General/Player.cpp
        src/General/Player.h
        src/General/Coin.cpp
        src/General/Coin.h

        # Ficheros UI
        src/Graficos/UIHandler.h
        src/Graficos/UIHandler.cpp

        # Parte Graficos
        src/Graficos/Renderer.h 
        src/Graficos/Renderer.cpp
        src/Graficos/Shader.h
        src/Graficos/Shader.cpp
        src/Graficos/Skybox.h
        src/Graficos/Skybox.cpp

        # Ficheros de Objecto
        src/Graficos/Modelo/Model.h
        src/Graficos/Modelo/Model.cpp
        src/Graficos/Modelo/Mesh.h
        src/Graficos/Modelo/Mesh.cpp
        src/Graficos/Modelo/Material.h
        src/Graficos/Modelo/Material.cpp
        src/Graficos/Modelo/Texture.h
        src/Graficos/Modelo/Texture.cpp 

        # Ficheros para la iluminacion
        src/Graficos/Luces/DirectionalLight.cpp
        src/Graficos/Luces/DirectionalLight.h
        src/Graficos/Luces/Light.cpp
        src/Graficos/Luces/Light.h
        src/Graficos/Luces/PointLight.cpp
        src/Graficos/Luces/PointLight.h
        src/Graficos/Luces/SpotLight.cpp
        src/Graficos/Luces/SpotLight.h

        src/Graficos/Sombras/ShadowMap.h
        src/Graficos/Sombras/ShadowMap.cpp
        src/Graficos/Sombras/OmniShadowMap.h
        src/Graficos/Sombras/OmniShadowMap.cpp

        # Ficheros para el sonido
        src/Music/MusicBuffer.cpp
        src/Music/MusicBuffer.h
        src/Music/SoundDevice.cpp
        src/Music/SoundDevice.h
        src/Music/SoundEffectsLibrary.cpp
        src/Music/SoundEffectsLibrary.h
        src/Music/SoundEffectsPlayer.cpp
        src/Music/SoundEffectsPlayer.h 
        src/Music/MySoundEffects.h 
        src/Music/MySoundEffects.cpp 
        src/Music/MySoundEffects.cpp)

# set(TEST_SOURCES ...)

add_executable(GameExecutable src/main.cpp ${GAME_SOURCES})

add_executable(MenuDemoExecutable src/main-menu.cpp ${GAME_SOURCES})

add_executable(TestExecutable ${GAME_SOURCES})

set(GAME_LIBRARIES OpenAL::OpenAL OpenGL::GL GLEW::GLEW glfw "${BULLET_LIBRARIES}" SndFile::sndfile soil2 ${ASSIMP_LIBRARIES} imgui::imgui)

target_link_libraries(GameExecutable  PRIVATE ${GAME_LIBRARIES})

target_link_libraries(MenuDemoExecutable  PRIVATE ${GAME_LIBRARIES})

target_link_libraries(TestExecutable  PRIVATE ${GAME_LIBRARIES} GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main) 

add_test(AllTestsInMain TestExecutable)

# Discover and run tests
gtest_discover_tests(TestExecutable)