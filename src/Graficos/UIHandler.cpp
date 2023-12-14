#include "UIHandler.h"

UIHandler::UIHandler(GLFWwindow* window) : window(window), m_progText(0), m_rotation_start_index(0) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    glfwGetFramebufferSize(window, &display_w, &display_h);

    //HABRIA QUE PONERLO EN UNA FUNCION A PARTE...
    io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../../src/Graficos/NFS.ttf", 15.0f);  // Cambia el nombre del archivo y el tamano segun tus necesidades
    m_HUDFont = io.Fonts->Fonts[0];


    // Load textures
    img_gameStarts = LoadTexture("../../../Assets/Imagenes/s2.jpg");
    img_help = LoadTexture("../../../Assets/Imagenes/s2.jpg");
    img_exit = LoadTexture("../../../Assets/Imagenes/s2.jpg");
    img_MMBackground = LoadTexture("../../../Assets/Imagenes/s2.jpg");
    img_LSBackground = LoadTexture("../../../Assets/Imagenes/LoadingScreenFoto.png");
    // Cargar texturas
    img_Speedometer = LoadTexture("../../../Assets/Imagenes/speedometer.png");
    img_SpeedPointer = LoadTexture("../../../Assets/Imagenes/pointer.png");
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

UIEvents UIHandler::DrawAndPollEvents(int flags, float data, float carSpeed, float rotAngle, int actCoin, int totalCoin)
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
		DrawPauseMenu(result);
    }
    if (flags & HUD) {
        //      timer,              carSpeed,           rotationAngle,          coinsCollected,     totalCoins
        DrawHUD(data, carSpeed, rotAngle, actCoin, totalCoin);
    }
    if (flags & Load_Screen) {
        DrawLoadScreen(result, data);
    }

    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
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
        std::cerr << "Error cargando textura " << path << std::endl;
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

void UIHandler::ImRotateStart()
{
	m_rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 UIHandler::ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = m_rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);


    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

ImVec2 operator-(const ImVec2& l, const ImVec2& r) { return{ l.x - r.x, l.y - r.y }; }
ImVec2 operator+(const ImVec2& l, const ImVec2& r) { return{ l.x + r.x, l.y + r.y }; }

void UIHandler::ImRotateEnd(float rad)
{
    ImVec2 center = ImRotationCenter();
    float desfase = M_PI / 1.35f;
    float s = sin(rad - desfase), c = cos(rad - desfase);

    center = ImRotate(center, s, c) - center;
    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = m_rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void UIHandler::ImRotateEnd_MinMax(float rad, int v)
{
    ImVec2 center = ImRotationCenter();
    float s, c;
    if (v == 0)
    {
        s = -0.735723, c = -0.677282;
    }
    else
    {
        s = -0.68631, c = 0.727309;
    }

    center = ImRotate(center, s, c) - center;
    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = m_rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}


void UIHandler::DrawHUD(float timer, float carSpeed, float rotationAngle, int coinsCollected, int totalCoins)
{
    ImGui::PushFont(m_HUDFont);

    //Desactivar las decoraciones de la ventana
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    //Quitar el fondo transparente
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, 50), ImGuiCond_Always);
    ImGui::Begin("Temporizador", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(2.0f);
    if (timer > 25.0f)
    {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Tiempo: %.2f", timer);
    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tiempo: %.2f", timer);
    }
   
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(900, 400), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(450, 500), ImGuiCond_Always);
    ImGui::Begin("Speedometer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
    //Mostrar la imagen del velocimetro
    ImVec2 speedometerSize(300.0f, 300.0f);  // Ajusta segun sea necesario
    ImGui::Image((void*)(intptr_t)img_Speedometer, speedometerSize);
    ImGui::End();


    ImGui::SetNextWindowPos(ImVec2(900, 400), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(450, 500), ImGuiCond_Always);
    ImGui::Begin("pointer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
    // Mostrar la imagen del indicador
    ImVec2 pointerSize(300.0f, 300.0f); // Ajusta seg?n sea necesario
    // Dibuja la imagen sin rotaci?n
    ImRotateStart();
    ImGui::Image((void*)(intptr_t)img_SpeedPointer, pointerSize);
    
    if (carSpeed > 150)
        ImRotateEnd_MinMax(-rotationAngle, 1);
    else if (carSpeed < -1)
        ImRotateEnd_MinMax(-rotationAngle, 0);
    else
        ImRotateEnd(-rotationAngle);

    ImGui::End();

    // Actualizar el contador de monedas en la interfaz
    ImGui::SetNextWindowPos(ImVec2(10, 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, 50), ImGuiCond_Always);

    ImGui::Begin("Contador de Monedas", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(2.0f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Monedas: %d / %d", coinsCollected, totalCoins + coinsCollected);
    ImGui::End();

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();
    ImGui::PopFont();
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





