//
// Created by jg03dev on 10/9/23.
//

#include "Window.h"

Window::Window()
{
    xChange = 0.0f;
    yChange = 0.0f;

    width = 800;
    height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    xChange = 0.0f;
    yChange = 0.0f;

    width = windowWidth;
    height = windowHeight;

    for(bool & key : keys)
        key = false;
}

int Window::Initialise()
{
    // Initialize GLFW
    if (glfwInit() != GLFW_TRUE)
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL vesron
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get the buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Handle Key + Mouse Input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    auto* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if(key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow *window, double xPos, double yPos)
{
    auto* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}


