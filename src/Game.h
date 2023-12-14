#pragma once

// --OUR INCLUDES--

#pragma region ENCABEZADOS

#include "Encabezados/stdafx.h"

#pragma endregion

#pragma region GENERAL

#include "General/Camera.h"
#include "General/Object.h"
#include "General/Player.h"
#include "General/Coin.h"

#pragma endregion

#pragma region GRAFICOS

// 3D
#include "Graficos/Renderer.h"
#include "Graficos/Skybox.h"
#include "Graficos/Luces/Light.h"

// UI
#include "Graficos/UIHandler.h"

#pragma endregion

#pragma region SONIDOS

#include "Music/SoundDevice.h"
#include "Music/MusicBuffer.h"
#include "Music/SoundEffectsLibrary.h"
#include "Music/SoundEffectsPlayer.h"
#include "Music/MySoundEffects.h"

#pragma endregion
#pragma region TEXTO
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma endregion

// --OTHER INCLUDES--

enum GameState {
	MainMenu,
	Loading,
	InGame,
	GameOver
};

class Game
{
public:
	// Public Attributes

	const char* name = "Nombre juego";

	// Constructors

	Game() :m_Window(NULL), m_renderer(NULL), m_Camera(NULL),
		m_dynamicsWorld(NULL), m_Player(NULL), m_sinTime(0), m_coinsCollected(0), m_lightsOn(false),
		m_currentState(GameState::MainMenu), m_deltaTime(0.0f), m_lastFrame(0.0f),
		m_sound(NULL), m_progressBar(0.01f){
		m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		InitializeWindow();
	};
	// Destructor
	~Game();

	// Game States
	void HandleMainMenu();
	void HandleLoading();
	GLFWwindow* CreateSharedGLFWWindow();
	void HandleGameOver();

	// Public Methods
	int Start();

	GLFWwindow* GetWindow() { return m_Window; }
	Camera* GetCamera() { return m_Camera; }

	void ProcessInput(GLFWwindow* window, int key, int action);
	void img_loader();

private:
	/// Private Attributes
	GameState m_currentState;
	// Window
	GLFWwindow* m_Window;
	const unsigned int m_SCR_WIDTH = 1280;
	const unsigned int m_SCR_HEIGHT = 720;

	// Camera
	Camera* m_Camera;
	const float c_near = 0.1f;
	const float c_far = 500.0f;

	// Graphics
	Renderer* m_renderer;

	// UI
	UIHandler* m_ui;

	// Physics logic
	float m_deltaTime;
	float m_lastFrame;
	btDiscreteDynamicsWorld* m_dynamicsWorld;

	// Objects
	Player* m_Player;
	std::vector<Object*> m_Objects;
	std::vector<Coin*> m_Coins;
	

	// Sound
	MySoundEffects* m_sound;

	//// Private Methods


	/// Animations
	void performJumpAndSpin(std::string id, float time1, float time2);
	//Time
	float m_sinTime;
	// Constants for animation parameters
	const float jumpHeight = 0.12f;
	const float jumpDuration = 2.0f;

	const float spinSpeed = 2.0f;

	/// Loading
	float m_progressBar;
	void UpdateProgressBar(); 
	// Contador Monedas
	int m_coinsCollected;

	// Initializers
	void InitializePhysics();
	void InitializeGraphics();
	void InitializeSound();
	int InitializeWindow();

	// Lights
	bool m_lightsOn;

	// LoadingScreen
	void DisplayLoadingScreen();


	/// Functionality
	void Run();

	float easeInOutQuad(float t) { return t < 0.5 ? 8 * pow(t, 4) : 1 - pow(-2 * t + 2, 4) / 2; }

	void Actualizar(float deltaTime);
	void Render();

	void LoadScreen();
};

