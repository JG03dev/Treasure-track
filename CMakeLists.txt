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

add_executable(VGI-ABP src/main.cpp
        src/Mesh.h
        src/Mesh.cpp
        src/Shader.cpp
        src/Shader.h
        src/Window.cpp
        src/Window.h
        src/Camera.cpp
        src/Camera.h        
        src/Entorn/objLoader.cpp
        src/Entorn/objLoader.h
        src/Entorn/escena.cpp
        src/Entorn/escena.h
        src/Entorn/constants.h
        src/Entorn/glut_geometry.cpp
        src/Entorn/glut_geometry.h
        "src/Material.cpp"
        "src/Material.h"
        src/StringHandler.cpp
        "src/stdafx.h"
        src/Entorn/visualitzacio.h
        src/Entorn/visualitzacio.cpp
        src/Entorn/quatern.cpp
        src/Entorn/quatern.h "src/Entorn/shader.cpp" "src/Entorn/shader.h" "src/skybox.cpp" "src/skybox.h") 

# target_link_libraries(VGI-ABP PRIVATE glfw "${ASSIMP_INCLUDE_DIRS}/assimp" "${BULLET_LIBRARIES}" GLEW::glew OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE OpenGL::GL)
target_link_libraries(VGI-ABP PRIVATE GLEW::GLEW)
target_link_libraries(VGI-ABP PRIVATE glfw)
target_link_libraries(VGI-ABP PRIVATE "${BULLET_LIBRARIES}")
target_link_libraries(VGI-ABP PRIVATE soil2)