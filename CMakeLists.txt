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
        src/stdafx.h
        src/Primitives.h
        src/Primitives.cpp
        
# Fiechros de funcionalidades del juego
        src/Camera.h
        src/Camera.cpp
        src/Skybox.h
        src/Skybox.cpp
        src/Shadow.h
        src/Shadow.cpp

# Ficheros de Objecto
        src/Model.h
        src/Model.cpp
        src/Shader.h
        src/Shader.cpp
        src/Mesh.h
        src/Mesh.cpp
        src/Texture.h
        src/Texture.cpp "src/Lights/DirectionalLight.cpp" "src/Lights/DirectionalLight.h" "src/Lights/Light.cpp" "src/Lights/Light.h" "src/Lights/PointLight.cpp" "src/Lights/PointLight.h" "src/Lights/SpotLight.cpp" "src/Lights/SpotLight.h" "src/stb_image.h")


# target_link_libraries(VGI-ABP PRIVATE glfw "${ASSIMP_INCLUDE_DIRS}/assimp" "${BULLET_LIBRARIES}" GLEW::glew OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenGL::GL)
target_link_libraries(VGI-ABP PRIVATE GLEW::GLEW)
target_link_libraries(VGI-ABP PRIVATE glfw)
target_link_libraries(VGI-ABP PRIVATE "${BULLET_LIBRARIES}")
target_link_libraries(VGI-ABP PRIVATE soil2)
target_link_libraries(VGI-ABP PRIVATE ${ASSIMP_LIBRARIES})