#include "../src/Encabezados/stdafx.h"
#include "Utility.hpp"
#include "TestCamera.hpp"
#include "TestLights.hpp"
#include "TestModel.hpp"
#include "TestRenderer.hpp"

#define WIDTH 800
#define HEIGHT 600

GLFWwindow* initWindow()
{
    GLFWwindow* m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Treasure Track", NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(m_Window);
    return m_Window;
}

int main()
{

    std::cout << "Starting Everything" << std::endl;
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

    GLFWwindow* w = initWindow();

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
    std::cout << "Everything Initialized" << std::endl;

    std::cout << "==Starting Unit Tests==" << std::endl;
    int fails = 0;
    std::cout << "Camera Tests:" << std::endl;
    fails += testCamera();

    std::cout << "Light Test" << std::endl;
    fails += testLights();

    std::cout << "Model Test" << std::endl;
    fails += testModels();

    std::cout << "Renderer Test" << std::endl;
    fails += testRenderer();

    if (fails != 0)
    {
        char error[100];
        sprintf(error, "Error, there were %d failed tests, aborting", fails);
        printFailMessage(error);

        return 1;
    }
    std::cout << "==Starting integration test, making scene==" << std::endl;

    Renderer r("../../../Tests/testIntegration.json", WIDTH, HEIGHT, 0, 0);
    std::cout << "Loading objects" << std::endl;
    for (auto o : r.getModelList())
        o.second.first->Load();
    std::cout << "Objects loaded" << std::endl;
    std::cout << "Rendering scene" << std::endl;

    Camera cam(glm::vec3(-10.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -10.0f);
    cam.changeCamera();//Activate first person
    cam.update();

    float frames = 0;

    while (!glfwWindowShouldClose(w))
    {
        glfwPollEvents();

        // Timer: close after 1000 frames
        if (frames >= 10000)
            glfwSetWindowShouldClose(w, true);
        frames++;

        // Rendering
        glm::mat4 projection = glm::perspective(glm::radians(cam.FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 500.0f);
        r.RenderEverything(cam, projection);

        glfwSwapBuffers(w);
    }
    printPassMessage("==All tests passed==");
    return 0;

}
