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
    UIEvents DrawAndPollEvents(int flags, float data = 0.0f);

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


    GLuint LoadTexture(const char* path);

    int m_progText;
    const char* loadingTexts[LOADING_STAGES] = {
      "Setting up Physics...",
      "Setting up Sounds...",
      "Setting up Graphics...",
      "Loading Models..."
    };

    //Draws (one per flag)
    void DrawMainMenu(UIEvents& e);
    void DrawPauseMenu(UIEvents& e);
    void DrawHUD(UIEvents& e);
    void DrawLoadScreen(UIEvents& e, float progress);

};