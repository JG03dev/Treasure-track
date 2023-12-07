#include "UIHandler.h"

UIHandler::UIHandler(GLFWwindow* window) : window(window) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Load textures
    mainMenuButtonTexture = LoadTexture("path/to/Assets/main_menu_button.png");
    pauseMenuButtonTexture = LoadTexture("path/to/Assets/pause_menu_button.png");
    hudButtonTexture = LoadTexture("path/to/Assets/hud_button.png");
}

UIHandler::~UIHandler() {
    glDeleteTextures(1, &mainMenuButtonTexture);
    glDeleteTextures(1, &pauseMenuButtonTexture);
    glDeleteTextures(1, &hudButtonTexture);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

UIEvents UIHandler::DrawAndPollEvents(UIFlags flags)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    //Warning! only one event can be handled at a time
    //This means result 
    UIEvents result = None;

    if (flags & Main_Menu) {
        DrawMainMenu(result);
    }
    if (flags & Pause_Menu) {
        DrawMainMenu(result);
    }
    if (flags & HUD) {
        DrawMainMenu(result);
    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return result;
}

GLuint UIHandler::LoadTexture(const char* path)
{
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

// Events

void UIHandler::DrawMainMenu(UIEvents& e)
{
}

void UIHandler::DrawPauseMenu(UIEvents& e)
{
}

void UIHandler::DrawHUD(UIEvents& e)
{
}



