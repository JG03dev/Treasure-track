/*
* This should return the Imported, the Device and the context from
* the libraries, but for now is to test that it compiles and links
* them correctly.
*/

// --OUR INCLUDES--

#pragma region ENCABEZADOS

#include "Encabezados/stdafx.h"

#pragma endregion

#pragma region GENERAL

#include "Game.h"
#include "General/Camera.h"

#pragma endregion

#pragma region GRAFICOS

#pragma endregion

#pragma region SONIDOS

#pragma endregion

// --OTHER INCLUDES--

#pragma region CAMCONTROL

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#pragma endregion
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
int main(int argc, char **argv) {
    std::cout << argv[0] << std::endl;
    int state = 0;

    // Intialize GLFW 
    // GLFW is configured.  Must be called before calling any GLFW functions
    if (!glfwInit()) {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    Game game;

    // Set the required callback functions
    glfwSetFramebufferSizeCallback(game.GetWindow(), glfw_onFramebufferSize);
    SoundDevice::Init();

    // Hide the cursor and capture it
    //img_loader();

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        // An error occured
        std::cerr << "GLEW initialization failed" << std::endl;
        std::cerr << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Depth test
    glEnable(GL_DEPTH_TEST);

    if (game.Start() != 0)
        std::cout << "Game crashed" << std::endl;

    SoundDevice::Release();

    return 0;
}

