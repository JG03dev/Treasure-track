
//Merge-Note: Este archivo no tendria que existir despues del merge ya que las globales deben de ser o bien constantes, estar en una classe (como game) o como varialbes locales en el main

#include "include/stdafx.h"
#include "General/Camera.h"

// Global Variables
const char* APP_TITLE = "VGI-ABP";
const int gWindowWidth = 1280;
const int gWindowHeight = 720;
GLFWwindow* gWindow = NULL;

//Light constants
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

// Camera system
Camera camera(glm::vec3(0.0f, 0.0f, 30.0f));
float c_near = 0.1f;
float c_far = 500.0f;

// Shader control
bool enableTorch = true;
bool enableNormal = true;
float adjustGamma = 2.2f;
float adjustParallax = 0.01f;