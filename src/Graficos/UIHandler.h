#pragma once

#include "../Encabezados/stdafx.h"
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>

// There can be multiple events drawn (ej. Pause and HUD)
enum UIFlags {
	DMainMenu = 1 << 0,
	DPauseMenu = 1 << 1,
	DHUD = 1 << 2,
	DLoadScreen = 1 << 3,
	DWonMenu = 1 << 4,
	DLostMenu = 1 << 5,
	DMenuModeSelection = 1 << 6,
    DMenuTimeSelection = 1 << 7,
    DEndScreen = 1 << 8,
    DCoinHUD = 1 << 9,
};

//Only ONE event will be handled per framexº
enum UIEvents {
    None,
    Start_Game,
    CoinMode,
    FreeMode,
    Day,
    Afternoon,
    Night,
    Help,
    Resume,
    Return,
    Exit
};

const int LOADING_STAGES = 4;

class UIHandler {
public:
    UIHandler(GLFWwindow* window);

    ~UIHandler();

    // Draw method to draw different UI setups based on flags
    UIEvents UIHandler::DrawAndPollEvents(int flags, float data = 0.0f, float carSpeed = 0.0f, float rotAngle = 0.0f, int actCoin = 0, int totalCoin = 0);

    void cycleLoadingTexts() { m_progText = m_progText + 1 < LOADING_STAGES ? m_progText + 1 : 0; }

	void StartContext() { 
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        ImGui::CreateContext();     
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");        
	}

private:
    //Window
    GLFWwindow* window;
    int display_w, display_h;

    // Buttons
    GLuint img_gameStarts;
    GLuint img_help;
    GLuint img_exit;
    GLuint img_CoinMode;
    GLuint img_FreeMode;
    GLuint img_Day;
    GLuint img_Afternoon;
    GLuint img_Night;

    // Backgrounds
    GLuint img_MMBackground;
    GLuint img_LSBackground;

    // Others
    GLuint img_Speedometer;
    GLuint img_SpeedPointer;
    GLuint img_Title;
    GLuint img_Controls;
    GLuint img_SaulGoodman;

    ImFont* m_HUDFont;
    ImGuiIO io;

    GLuint LoadTexture(const char* path);

    int m_progText;
    const char* loadingTexts[LOADING_STAGES] = {
      "Setting up Physics...",
      "Setting up Sounds...",
      "Setting up Graphics...",
      "Loading Models..."
    };

    // HUD logic
    int m_rotation_start_index;
    void ImRotateStart();
    ImVec2 ImRotationCenter();
    void ImRotateEnd(float rad);
    void ImRotateEnd_MinMax(float rad, int v);

    //Draws (one per flag)
    void DrawMainMenu(UIEvents& e);
    void DrawModeMenu(UIEvents& e);
    void DrawTimeMenu(UIEvents& e);
    void DrawPauseMenu(UIEvents& e);
    void DrawHUD(float carSpeed, float rotationAngle);
    void DrawCoinHUD(float timer, int coinsCollected, int totalCoins);
    void DrawLoadScreen(UIEvents& e, float progress);
	void DrawEndScreen(UIEvents& e, int coinsCollected, int totalCoins);
};