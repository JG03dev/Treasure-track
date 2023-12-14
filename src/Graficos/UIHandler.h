#pragma once

#include "../Encabezados/stdafx.h"
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>

// There can be multiple events drawn (ej. Pause and HUD)
enum UIFlags {
    Main_Menu = 1 << 0,
    Pause_Menu = 1 << 1,
    HUD = 1 << 2,
    Load_Screen = 1 << 3,
};

//Only ONE event will be handled per frame
enum UIEvents {
    None,
    Start_Game,
    Help,
    Resume,
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

private:
    //Window
    GLFWwindow* window;
    int display_w, display_h;


    GLuint img_gameStarts;
    GLuint img_help;
    GLuint img_exit;
    GLuint img_MMBackground;
    GLuint img_LSBackground;
    GLuint img_Speedometer;
    GLuint img_SpeedPointer;


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
    void DrawPauseMenu(UIEvents& e);
    void DrawHUD(float timer, float carSpeed, float rotationAngle, int coinsCollected, int totalCoins);
    void DrawLoadScreen(UIEvents& e, float progress);

};