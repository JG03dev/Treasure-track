#include "UIHandler.h"

UIHandler::UIHandler(GLFWwindow* window) : window(window), m_progText(0) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glfwGetFramebufferSize(window, &display_w, &display_h);

    // Load textures
    img_gameStarts = LoadTexture("../../../src/s2.jpg");
    img_help = LoadTexture("../../../src/s2.jpg");
    img_exit = LoadTexture("../../../src/s2.jpg");
    img_MMBackground = LoadTexture("../../../src/s2.jpg");
    img_LSBackground = LoadTexture("../../../src/LoadingScreenFoto.png");
}

UIHandler::~UIHandler() {
    glDeleteTextures(1, &img_gameStarts);
    glDeleteTextures(1, &img_help);
    glDeleteTextures(1, &img_exit);
    glDeleteTextures(1, &img_MMBackground);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

UIEvents UIHandler::DrawAndPollEvents(int flags, float data)
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
    if (flags & Load_Screen) {
        DrawLoadScreen(result, data);
    }

    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    // Set the background image
    ImVec2 windowSize(display_w, display_h);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Image((void*)(intptr_t)img_MMBackground, windowSize);
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
    if (ImGui::ImageButton((void*)(intptr_t)img_gameStarts, ImVec2(200, 50))) {
        e = Start_Game;
    }

    // Help button
    ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 100));  // Adjust the vertical distance
    if (ImGui::ImageButton((void*)(intptr_t)img_help, ImVec2(200, 50))) {
        e = Help;
    }

    // Exit button
    ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 200));  // Adjust the vertical distance
    if (ImGui::ImageButton((void*)(intptr_t)img_exit, ImVec2(200, 50))) {
        e = Exit;
    }

    ImGui::End();
}

void UIHandler::DrawPauseMenu(UIEvents& e)
{
}

void UIHandler::DrawHUD(UIEvents& e)
{
}

void UIHandler::DrawLoadScreen(UIEvents& e, float progress) {
    // Set the background image
    ImVec2 windowSize(display_w, display_h);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Image((void*)(intptr_t)img_LSBackground, windowSize);
    ImGui::End();

    // Set up the loading bar
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0, display_h - 30)); // Bottom of the screen
    ImGui::Begin("Loading Bar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);

    // Set text color to white
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Draw white text
    ImGui::Text(loadingTexts[m_progText]);

    // Pop the text color change
    ImGui::PopStyleColor();

    ImGui::ProgressBar(progress, ImVec2(-1, 0));

    ImGui::End();
}





