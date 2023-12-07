#pragma once

#include "../Encabezados/stdafx.h"

// There can be multiple events drawn (ej. Pause and HUD)
enum UIFlags {
    Main_Menu = 1 << 0,
    Pause_Menu = 1 << 1,
    HUD = 1 << 2,
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
    UIEvents DrawAndPollEvents(UIFlags flags);

private:
    //Window
    GLFWwindow* window;


    GLuint mainMenuButtonTexture;
    GLuint pauseMenuButtonTexture;
    GLuint hudButtonTexture;


    GLuint LoadTexture(const char* path);

    //Draws (one per flag)
    void DrawMainMenu(UIEvents& e);
    void DrawPauseMenu(UIEvents& e);
    void DrawHUD(UIEvents& e);


};