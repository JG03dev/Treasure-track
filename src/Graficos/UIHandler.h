#pragma once

#include "../Encabezados/stdafx.h"

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

class UIHandler {
public:
    UIHandler(GLFWwindow* window);

    ~UIHandler();

    // Draw method to draw different UI setups based on flags
    UIEvents DrawAndPollEvents(int flags);

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

    float progress;

    //Draws (one per flag)
    void DrawMainMenu(UIEvents& e);
    void DrawPauseMenu(UIEvents& e);
    void DrawHUD(UIEvents& e);
    void DrawLoadScreen(UIEvents& e);

};