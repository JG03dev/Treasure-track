cmake_minimum_required(VERSION 3.26)
project(VGI-ABP)

set(CMAKE_CXX_STANDARD 14)
IF (WIN32)
    # Add the path to the vcpkg toolchain file
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
ENDIF()

find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(assimp REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(VGI-ABP src/main.cpp
        src/Mesh.h
        src/Mesh.cpp
        src/Shader.cpp
        src/Shader.h
        src/Window.cpp
        src/Window.h
        src/Camera.cpp
        src/Camera.h)

# target_link_libraries(VGI-ABP PRIVATE glfw "${ASSIMP_INCLUDE_DIRS}/assimp" "${BULLET_LIBRARIES}" GLEW::glew OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(VGI-ABP PRIVATE OpenAL::OpenAL)
target_link_libraries(VGI-ABP PRIVATE GLEW::GLEW)
target_link_libraries(VGI-ABP PRIVATE glfw)