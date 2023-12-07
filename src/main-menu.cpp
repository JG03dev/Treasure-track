/*
* This should return the Imported, the Device and the context from
* the libraries, but for now is to test that it compiles and links
* them correctly.
*/

// --OUR INCLUDES--
#define STB_IMAGE_IMPLEMENTATION    

#pragma region ENCABEZADOS

#include "Encabezados/stdafx.h"

#pragma endregion

#pragma region GENERAL

#pragma endregion

#pragma region GRAFICOS
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#pragma endregion

#pragma region SONIDOS

#pragma endregion



// --OTHER INCLUDES--

//Game* g = nullptr;
GLFWwindow* m_Window;
const unsigned int m_SCR_WIDTH = 1280;
const unsigned int m_SCR_HEIGHT = 720;

void img_loader(GLFWwindow* m_Window) {
    int width, height;
    int channels;
    unsigned char* pixels = stbi_load("../../../src/s2.jpg", &width, &height, &channels, 4);

    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(m_Window, 1, images);
}

GLuint LoadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (data) {
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else {
        // Handle image loading failure
    }

    return textureID;
}

int main(int argc, char** argv) {
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

    // glfw window creation
    // --------------------
    GLFWwindow* m_Window = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "VGI-ABP", NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_Window);

    // Hide the cursor and capture it
    img_loader(m_Window);

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
    GLuint Background = LoadTexture("../../../src/s2.jpg");
    GLuint GameStarts = LoadTexture("../../../src/s2.jpg");
    GLuint Help = LoadTexture("../../../src/s2.jpg");
    GLuint Exit = LoadTexture("../../../src/s2.jpg");


    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(m_Window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImVec2 imageSize(100, 100);

        // Set the background image
        ImVec2 windowSize = ImVec2(m_SCR_WIDTH, m_SCR_HEIGHT);  // Set your window size
        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Image((void*)(intptr_t)Background, windowSize);
        ImGui::End();

        // Set up the main menu
        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

        ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);
        // Calculate the center position for the buttons
        ImVec2 centerPos = ImVec2((windowSize.x - 200) * 0.5f, (windowSize.y - 150) * 0.5f);

        // Start button
        ImGui::SetCursorPos(centerPos);
        if (ImGui::ImageButton((void*)(intptr_t)GameStarts, ImVec2(200, 50))) {
            // Handle start button click
            // Add logic to start the game
            std::cout << "GAME STARTS!" << std::endl;
        }

        // Help button
        ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 100));  // Adjust the vertical distance
        if (ImGui::ImageButton((void*)(intptr_t)Help, ImVec2(200, 50))) {
            // Handle help button click
            // Add logic to show help
            std::cout << "NEED HELP?" << std::endl;

        }

        // Exit button
        ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 200));  // Adjust the vertical distance
        if (ImGui::ImageButton((void*)(intptr_t)Exit, ImVec2(200, 50))) {
            // Handle exit button click
            std::cout << "GOODBYE!" << std::endl;
            glfwSetWindowShouldClose(m_Window, true);
        }

        ImGui::End();



        // Render ImGui
        ImGui::Render();
        glViewport(0, 0, (int)m_SCR_WIDTH, (int)m_SCR_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(m_Window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();

    return 0;
}

