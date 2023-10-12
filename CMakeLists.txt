cmake_minimum_required(VERSION 3.26)
project(OpenGLCourse)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(OpenGLCourse src/main.cpp
        src/Mesh.h
        src/Mesh.cpp
        src/Shader.cpp
        src/Shader.h
        src/Window.cpp
        src/Window.h
        src/Camera.cpp
        src/Camera.h)

target_link_libraries(OpenGLCourse PRIVATE OpenGL::GL glfw GLEW)