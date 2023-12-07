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
#include "General/Object.h"
#include "General/Player.h"

#pragma endregion

#pragma region GRAFICOS

#include "Graficos/skybox.h"
#include "Graficos/Shader.h"
#include "Graficos/Modelo/Model.h"
#include "Graficos/Luces/Light.h"

#pragma endregion

#pragma region TEXTO
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

void img_loader() {
    int width, height;
    int channels;
    unsigned char* pixels = stbi_load("../../../src/s2.jpg", &width, &height, &channels, 4);

    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(g->GetWindow(), 1, images);
}

int menu()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Configurar ImGui para OpenGL
    ImGui_ImplGlfw_InitForOpenGL(g->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Textura para la imagen de fondo
    GLuint backgroundTexture = 0;  // Identificador de textura 
    static bool show_start_menu = true;

    // Bucle principal
    while (!glfwGetKey(g->GetWindow(), GLFW_KEY_ENTER)) {
        glfwPollEvents();

        // Iniciar el marco de ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Renderizar la imagen de fondo usando OpenGL
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Establecer el color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        // Crear el menú de inicio
        if (show_start_menu) {
            ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
            ImGui::Begin("Ventana", &show_start_menu);

            ImGui::Text("HOLA!");

            ImGui::End();
        }

        // Renderizar el contenido de ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Intercambiar el búfer y mostrar el resultado en la ventana
        glfwSwapBuffers(g->GetWindow());
    }

    // Limpiar ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

int main(int argc, char **argv) {
    std::cout << argv[0] << std::endl;
    // Intialize GLFW 
    // GLFW is configured.  Must be called before calling any GLFW functions
    if (!glfwInit()) {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
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
    glfwSetInputMode(g->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    //menu();
    g->StartGame();

    SoundDevice::Release();
    delete g;

    return 0;
}

