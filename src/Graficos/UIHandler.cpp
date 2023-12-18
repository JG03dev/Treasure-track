#include "UIHandler.h"

UIHandler::UIHandler(GLFWwindow* window) : window(window), m_progText(0), m_rotation_start_index(0), showHelp(false) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    glfwGetFramebufferSize(window, &display_w, &display_h);

    //HABRIA QUE PONERLO EN UNA FUNCION A PARTE...
    io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../../src/Graficos/NFS.ttf", 15.0f);  // Cambia el nombre del archivo y el tamano segun tus necesidades
    m_HUDFont = io.Fonts->Fonts[0];

    // Load Button textures
    img_gameStarts = LoadTexture("../../../Assets/Imagenes/button_play.png");
    img_help = LoadTexture("../../../Assets/Imagenes/Controls-SinFondo.png");
    img_exit = LoadTexture("../../../Assets/Imagenes/button_exit.png");
    img_CoinMode = LoadTexture("../../../Assets/Imagenes/Coin-Collection-Challenge-SinFondo.png");
    img_FreeMode = LoadTexture("../../../Assets/Imagenes/MODO-LIBRE-SinFondo.png");
    img_Day = LoadTexture("../../../Assets/Imagenes/Dia-SinFondo.png");
	img_Afternoon = LoadTexture("../../../Assets/Imagenes/Tarde-SinFondo.png");
    img_Night = LoadTexture("../../../Assets/Imagenes/Noche-SinFondo.png");
    img_Close = LoadTexture("../../../Assets/Imagenes/CerrarIcono.png");
    img_Victory = LoadTexture("../../../Assets/Imagenes/VICTORIA.png");
    img_Defeat = LoadTexture("../../../Assets/Imagenes/DERROTA.png");

    // Load Background textures
    img_MMBackground = LoadTexture("../../../Assets/Imagenes/fondo.png");
    img_LSBackground = LoadTexture("../../../Assets/Imagenes/LoadingScreenFoto.png");

    // Load Other textures
    img_Speedometer = LoadTexture("../../../Assets/Imagenes/speedometer.png");
    img_SpeedPointer = LoadTexture("../../../Assets/Imagenes/pointer.png");
    img_Title = LoadTexture("../../../Assets/Imagenes/title6.png");
    img_Controls = LoadTexture("../../../Assets/Imagenes/Controles.png");
    img_SaulGoodman = LoadTexture("../../../Assets/Imagenes/s2.jpg");
}

UIHandler::~UIHandler() {
    glDeleteTextures(1, &img_gameStarts);
    glDeleteTextures(1, &img_help);
    glDeleteTextures(1, &img_exit);
    glDeleteTextures(1, &img_CoinMode);
	glDeleteTextures(1, &img_FreeMode);
	glDeleteTextures(1, &img_Day);
	glDeleteTextures(1, &img_Afternoon);
	glDeleteTextures(1, &img_Night);
	glDeleteTextures(1, &img_MMBackground);
	glDeleteTextures(1, &img_LSBackground);
	glDeleteTextures(1, &img_Speedometer);
	glDeleteTextures(1, &img_SpeedPointer);
	glDeleteTextures(1, &img_Title);
	glDeleteTextures(1, &img_Controls);

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
    
    if (flags & DMainMenu) {
        DrawMainMenu(result);
    }
    if (flags & DPauseMenu) {
		DrawPauseMenu(result);
    }
    if (flags & DCoinHUD) {
        //      timer, coinsCollected, totalCoins
        DrawCoinHUD(data, actCoin, totalCoin);
    }
    if (flags & DHUD) {
        //      carSpeed, rotationAngle,
        DrawHUD(carSpeed, rotAngle);
    }
    if (flags & DLoadScreen) {
        DrawLoadScreen(result, data);
    }
    if (flags & DMenuModeSelection) {
        DrawModeMenu(result);
    }
    if (flags & DMenuTimeSelection) {
        DrawTimeMenu(result);
    }
    if (flags & DEndScreen) {
        DrawEndScreen(result, actCoin, totalCoin);
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
	// Cambia el color del botón a transparente
	ImVec4 transparentColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Button] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = transparentColor;

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
	ImGui::SetCursorPos(ImVec2(centerPos.x - 250, centerPos.y - 250));
	ImGui::Image((void*)(intptr_t)img_Title, ImVec2(877 * 0.8f, 197 * 0.8f));

    if (!showHelp) {
        // Start button
        ImGui::SetCursorPos(centerPos);

        if (ImGui::ImageButton((void*)(intptr_t)img_gameStarts, ImVec2(200, 50))) {
            e = Start_Game;
        }

        // Help button
        ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 100));  // Adjust the vertical distance
        if (ImGui::ImageButton((void*)(intptr_t)img_help, ImVec2(200, 50))) {
            showHelp = !showHelp;
        }

        // Exit button
        ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 200));  // Adjust the vertical distance
        if (ImGui::ImageButton((void*)(intptr_t)img_exit, ImVec2(200, 50))) {
            e = Exit;
        }
    }
    else {
        ImGui::SetCursorPos(ImVec2(160, 90));
        ImGui::Image((void*)(intptr_t)img_Controls, ImVec2(1920/2, 1080/2));

        ImGui::SetCursorPos(ImVec2(1052, 88));
        if (ImGui::ImageButton((void*)(intptr_t)img_Close, ImVec2(512/8, 512/8))) {
            showHelp = !showHelp;
        }
    }

    ImGui::End();
}

void UIHandler::DrawModeMenu(UIEvents& e) {
	// Cambia el color del botón a transparente
	ImVec4 transparentColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Button] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = transparentColor;

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
    ImGui::Begin("Mode Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

    ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);
    // Calculate the center position for the buttons
    ImVec2 centerPos = ImVec2((windowSize.x - 200) * 0.5f, (windowSize.y - 150) * 0.5f);
    
    // Coin Mode Button
    ImGui::SetCursorPos(ImVec2(centerPos.x - 400, centerPos.y));
    if (ImGui::ImageButton((void*)(intptr_t)img_CoinMode, ImVec2(200, 200))) {
        e = CoinMode;
    }

    // Free Mode Button
    ImGui::SetCursorPos(ImVec2(centerPos.x + 400, centerPos.y));  // Adjust the vertical distance
    if (ImGui::ImageButton((void*)(intptr_t)img_FreeMode, ImVec2(200, 200))) {
        e = FreeMode;
    }

    ImGui::End();
}

void UIHandler::DrawTimeMenu(UIEvents& e) {
	// Cambia el color del botón a transparente
	ImVec4 transparentColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Button] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = transparentColor;

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
	ImGui::Begin("Time Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);
	// Calculate the center position for the buttons
	ImVec2 centerPos = ImVec2((windowSize.x - 200) * 0.5f, (windowSize.y - 150) * 0.5f);

	// Day Time Button
	ImGui::SetCursorPos(ImVec2(centerPos.x - 600, centerPos.y));
	if (ImGui::ImageButton((void*)(intptr_t)img_Day, ImVec2(200, 200))) {
		e = Day;
	}

	// Afternoon Time Button
	ImGui::SetCursorPos(centerPos);  // Adjust the vertical distance
	if (ImGui::ImageButton((void*)(intptr_t)img_Afternoon, ImVec2(200, 200))) {
		e = Afternoon;
	}

	// Night Time Button
	ImGui::SetCursorPos(ImVec2(centerPos.x + 600, centerPos.y));  // Adjust the vertical distance
	if (ImGui::ImageButton((void*)(intptr_t)img_Night, ImVec2(200, 200))) {
		e = Night;
	}

	ImGui::End();
}

void UIHandler::DrawEndScreen(UIEvents& e, int coinsCollected, int totalCoins) {
	// Cambia el color del botón a transparente
	ImVec4 transparentColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Button] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = transparentColor;

	// Set the background image
	ImVec2 windowSize(display_w, display_h);

	// Set up the main menu
	ImGui::SetNextWindowSize(windowSize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("End Screen", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);
	// Calculate the center position for the buttons
	ImVec2 centerPos = ImVec2((windowSize.x - 200) * 0.5f, (windowSize.y - 150) * 0.5f);

    ImGui::SetCursorPos(centerPos);
    if (coinsCollected == totalCoins) {
        if (ImGui::ImageButton((void*)(intptr_t)img_Victory, ImVec2(400, 200))) {
            e = Return;
        }
    }
    else {
        if (ImGui::ImageButton((void*)(intptr_t)img_Defeat, ImVec2(400, 200))) {
            e = Return;
        }
    }

	ImGui::End();
}

void UIHandler::DrawPauseMenu(UIEvents& e)
{
	// Cambia el color del botón a transparente
	ImVec4 transparentColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Button] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = transparentColor;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = transparentColor;

    // Set the background image
	ImVec2 windowSize(display_w, display_h);

	// Set up the main menu
	ImGui::SetNextWindowSize(windowSize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Pause Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	ImGui::SetNextWindowSizeConstraints(windowSize, windowSize);
	// Calculate the center position for the buttons
	ImVec2 centerPos = ImVec2((windowSize.x - 200) * 0.5f, (windowSize.y - 150) * 0.5f);

	// Resume Button
	ImGui::SetCursorPos(centerPos);
	if (ImGui::ImageButton((void*)(intptr_t)img_gameStarts, ImVec2(200, 50))) {
		e = Resume;
	}	

	// Return Button
	ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 100));  // Adjust the vertical distance
	if (ImGui::ImageButton((void*)(intptr_t)img_Afternoon, ImVec2(200, 50))) {
		e = Return;
	}

	// Exit Button
	ImGui::SetCursorPos(ImVec2(centerPos.x, centerPos.y + 200));  // Adjust the vertical distance
	if (ImGui::ImageButton((void*)(intptr_t)img_exit, ImVec2(200, 50))) {
		e = Exit;
	}

    // Controls Image
    ImGui::SetCursorPos(ImVec2(775, 250));  // Adjust the vertical distance
    ImGui::Image((void*)(intptr_t)img_Controls, ImVec2(1920/4, 1080/4));

	ImGui::End();
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


void UIHandler::DrawHUD(float carSpeed, float rotationAngle)
{
    ImGui::PushFont(m_HUDFont);

    //Desactivar las decoraciones de la ventana
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    //Quitar el fondo transparente
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

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
    // Dibuja la imagen sin rotacion
    ImRotateStart();
    ImGui::Image((void*)(intptr_t)img_SpeedPointer, pointerSize);
    
    if (carSpeed > 150)
        ImRotateEnd_MinMax(-rotationAngle, 1);
    else if (carSpeed < -1)
        ImRotateEnd_MinMax(-rotationAngle, 0);
    else
        ImRotateEnd(-rotationAngle);

    ImGui::End();

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();
    ImGui::PopFont();
}

void UIHandler::DrawCoinHUD(float timer, int coinsCollected, int totalCoins) {
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

	// Actualizar el contador de monedas en la interfaz
	ImGui::SetNextWindowPos(ImVec2(10, 50), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(250, 50), ImGuiCond_Always);

	ImGui::Begin("Contador de Monedas", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowFontScale(2.0f);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Monedas: %d / %d", coinsCollected, totalCoins);
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