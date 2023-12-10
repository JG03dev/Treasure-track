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

#include "Graficos/Renderer.h"
#include "Graficos/Skybox.h"
#include "Graficos/Luces/Light.h"

#pragma endregion

#pragma region SONIDOS

#include "Music/SoundDevice.h"
#include "Music/MusicBuffer.h"
#include "Music/SoundEffectsLibrary.h"
#include "Music/SoundEffectsPlayer.h"
#include "Music/MySoundEffects.h"

#pragma endregion


// --OTHER INCLUDES--


class Game
{
public: 
	// Public Attributes

	const char* name = "Nombre juego";

	// Constructors

	Game() :m_Window(NULL), m_Camera(NULL), m_skybox(NULL), m_Light(NULL), m_dynamicsWorld(NULL), m_Player(NULL), m_sinTime(0), m_coinsCollected(0) { m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f)); InitializeWindow(); };

	// Destructors

	~Game();
	
	// Public Methods

	void StartGame();

	GLFWwindow* GetWindow() { return m_Window; }
	Camera* GetCamera() { return m_Camera; }

	void ProcessInput(GLFWwindow* window, int key, int action);
	void img_loader();
	
private:
	// Private Attributes
	
	// Window
	GLFWwindow* m_Window;
	const unsigned int m_SCR_WIDTH = 1920;
	const unsigned int m_SCR_HEIGHT = 1080;

	// Camera
	Camera* m_Camera;
	const float c_near = 0.1f;
	const float c_far = 500.0f;


	///// THIS SHOULD BE ALL HANDLED BY THE RENDERER

	// Skybox
	Skybox* m_skybox;

	// Graphics
	Renderer* m_renderer;
	// These next 3 attributes will probably be stored in the Renderer
	Light* m_Light;
	glm::vec3 m_lightPos;

	/////

	// Physics logic
	float m_deltaTime;
	float m_lastFrame;
	btDiscreteDynamicsWorld* m_dynamicsWorld;

	// Objects
	Player* m_Player;
	std::vector<Object*> m_Objects;
	std::vector<Coin*> m_Coins;

	// Private Methods

	// Animacio moneda
	void performJumpAndSpin(std::string id, float time1, float time2);
	//Time
	float m_sinTime;
	// Constants for animation parameters
	const float jumpHeight = 0.12f;
	const float jumpDuration = 2.0f;

	const float spinSpeed = 2.0f;

	// Contador Monedas
	int m_coinsCollected;

	// Initializers
	void InitializePhysics();
	void InitializeGraphics();
	int InitializeWindow();

	// LoadingScreen
	void DisplayLoadingScreen();

	// Functionality
	void Run();
	
	void Actualizar(float deltaTime, MySoundEffects& sound);
	void Render();
};