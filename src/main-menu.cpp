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
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>
#pragma endregion

#pragma region GRAFICOS
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#pragma endregion

#pragma region SONIDOS

#pragma endregion

std::mutex g_mutex;
float g_totalProgress = 0.0f; 
bool g_loadingFinished = false;

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

// Function to simulate loading in a separate thread
void SimulateLoad(float& progress, const char* loadingText)
{
    while (progress < 1.0f)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        progress += 0.1f; // Simulate progress
    }

    // Notify the main thread that loading is finished
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_totalProgress += 1.0f; // Increment the total progress
        g_loadingFinished = true;
    }

    // Print loading text
    std::cout << loadingText << " - Loading complete." << std::endl;
}

// Custom ease function (you can experiment with different easing functions)
float CustomEase(float t)
{
    // Example: ease out quadratic
    return t * (2.0f - t);
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
    GLuint Background = LoadTexture("../../../src/fondo.png");
    GLuint Title = LoadTexture("../../../src/title6.png");
    GLuint Controles = LoadTexture("../../../src/Controles.jpg");

    GLuint Play = LoadTexture("../../../src/button_play.png"); 
    GLuint Info = LoadTexture("../../../src/button_info.png"); 
    GLuint Exit = LoadTexture("../../../src/button_exit.png");
    GLuint Continue = LoadTexture("../../../src/button_continue.png");

    GLuint Play_press = LoadTexture("../../../src/button_play_press.png"); 
    GLuint Info_press = LoadTexture("../../../src/button_info_press.png"); 
    GLuint Exit_press = LoadTexture("../../../src/button_exit_press.png"); 
    GLuint Continue_press = LoadTexture("../../../src/button_continue_press.png");

    bool hover = false; //para mirar si esta presionado o no

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
            ImVec2 windowSize = ImVec2(m_SCR_WIDTH, m_SCR_HEIGHT);
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

        ImGui::SetCursorPos(ImVec2(centerPos.x - 250, centerPos.y - 150));
        ImGui::Image((void*)(intptr_t)Title, ImVec2(877 * 0.8f, 197 * 0.8f)); 

        ImGuiStyle& style = ImGui::GetStyle(); 

        // Cambia el canal alfa del color de fondo de los botones a un valor transparente (0.0)
        ImVec4 transparentColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); 
        style.Colors[ImGuiCol_Button] = transparentColor; 
        style.Colors[ImGuiCol_ButtonHovered] = transparentColor;
        style.Colors[ImGuiCol_ButtonActive] = transparentColor;
       
        // Start button
        if (ImGui::IsMouseHoveringRect(centerPos, ImVec2(centerPos.x + 200, centerPos.y + 50))) {
            hover = true;
        }
        else {
            hover = false;
        }

        ImGui::SetCursorPos(centerPos);
        if (!hover) {
            ImGui::ImageButton((void*)(intptr_t)Play, ImVec2(200, 50));
        }
        else {
            if (ImGui::ImageButton((void*)(intptr_t)Play_press, ImVec2(200, 50)))
            {
                std::cout << "PLAY" << std::endl;
                //iniciar el juego
            }
        }


        //// Exit button
        if (ImGui::IsMouseHoveringRect(ImVec2(centerPos.x, centerPos.y + 80), ImVec2(centerPos.x + 200, centerPos.y + 80 + 50))) {
            hover = true;
        }
        else {
            hover = false;
        }

        ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 80));
        if (!hover) {
            ImGui::ImageButton((void*)(intptr_t)Exit, ImVec2(200, 50));
        }
        else {
            if (ImGui::ImageButton((void*)(intptr_t)Exit_press, ImVec2(200, 50)))
            {
                std::cout << "GOODBYE!" << std::endl;
                glfwSetWindowShouldClose(m_Window, true);
            }
        }

        // Help button
        if (ImGui::IsMouseHoveringRect(ImVec2(centerPos.x + 650, centerPos.y + 350), ImVec2(centerPos.x + 650 + 50, centerPos.y + 350 + 50))) {
            hover = true;
        }
        else {
            hover = false;
        }

        ImGui::SetCursorPos(ImVec2(centerPos.x + 650, centerPos.y + 350));
        if (!hover) {
            ImGui::ImageButton((void*)(intptr_t)Info, ImVec2(50, 50)); 
        }
        else {
            if (ImGui::ImageButton((void*)(intptr_t)Info_press, ImVec2(50, 50))) 
            { 
                std::cout << "Need Help?" << std::endl;
                ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y)); //poner
                ImGui::Image((void*)(intptr_t)Controles, ImVec2(1600 * 0.8f, 900 * 0.8f));
            }
        }

        // Menu pausa
        //// Continue button
        //if (ImGui::IsMouseHoveringRect(centerPos, ImVec2(centerPos.x + 200, centerPos.y + 50))) {
        //    hover = true;
        //}
        //else {
        //    hover = false;
        //}

        //ImGui::SetCursorPos(centerPos);
        //if (!hover) {
        //    ImGui::ImageButton((void*)(intptr_t)Continue, ImVec2(200, 50));
        //}
        //else {
        //    if (ImGui::ImageButton((void*)(intptr_t)Continue_press, ImVec2(200, 50)))
        //    {
        //        std::cout << "Continua" << std::endl;
        //        //continuar el juego
        //    }
        //}


        ////// Exit button
        //if (ImGui::IsMouseHoveringRect(ImVec2(centerPos.x, centerPos.y + 80), ImVec2(centerPos.x + 200, centerPos.y + 80 + 50))) {
        //    hover = true;
        //}
        //else {
        //    hover = false;
        //}

        //ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 80));
        //if (!hover) {
        //    ImGui::ImageButton((void*)(intptr_t)Exit, ImVec2(200, 50));
        //}
        //else {
        //    if (ImGui::ImageButton((void*)(intptr_t)Exit_press, ImVec2(200, 50)))
        //    {
        //        std::cout << "GOODBYE!" << std::endl;
        //        glfwSetWindowShouldClose(m_Window, true);
        //    }
        //}

        //// Help button
        //if (ImGui::IsMouseHoveringRect(ImVec2(centerPos.x + 650, centerPos.y + 350), ImVec2(centerPos.x + 650 + 50, centerPos.y + 350 + 50))) {
        //    hover = true;
        //}
        //else {
        //    hover = false;
        //}

        //ImGui::SetCursorPos(ImVec2(centerPos.x + 650, centerPos.y + 350));
        //if (!hover) {
        //    ImGui::ImageButton((void*)(intptr_t)Info, ImVec2(50, 50));
        //}
        //else {
        //    if (ImGui::ImageButton((void*)(intptr_t)Info_press, ImVec2(50, 50)))
        //    {
        //        std::cout << "Need Help?" << std::endl;
        //        ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y)); //poner
        //        ImGui::Image((void*)(intptr_t)Controles, ImVec2(1600 * 0.8f, 900 * 0.8f));
        //    }
        //}

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

        // Join the loading thread
        loadingThread.join();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();

    return 0;
}

