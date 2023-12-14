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

Game* g = nullptr;

#pragma region CAMCONTROL

Camera* camera; //Camera global (TEMPORAL) necessario para obtener la camera de game para las funciones de glfw
int gWindowWidth, gWindowHeight;


//-----------------------------------------------------------------------------
// Is called whenever mouse movement is detected via GLFW
//-----------------------------------------------------------------------------
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

    static bool  firstMouse = true;
    static float lastX = gWindowWidth / 2;
    static float lastY = gWindowHeight / 2;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coord range from buttom to top
    lastX = xpos;
    lastY = ypos;

    camera->processMouse(xoffset, yoffset);
}

//-----------------------------------------------------------------------------
// Is called whenever scroller is detected via GLFW
//-----------------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera->processScroll(yoffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    g->ProcessInput(window, key, action);
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#pragma endregion

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

    g = new Game();
    camera = g->GetCamera();

    // Set the required callback functions
    glfwSetKeyCallback(g->GetWindow(), keyCallback);
    glfwSetCursorPosCallback(g->GetWindow(), mouseCallback);
    glfwSetScrollCallback(g->GetWindow(), scrollCallback);
    glfwSetFramebufferSizeCallback(g->GetWindow(), glfw_onFramebufferSize);
    SoundDevice::Init();

    // Hide the cursor and capture it
    img_loader();

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

    if (g->Start() != 0)
        std::cout << "Game crashed" << std::endl;

    SoundDevice::Release();
    delete g;

    return 0;
}

