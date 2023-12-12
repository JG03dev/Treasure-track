#pragma once

// --OUR INCLUDES--

#pragma region ENCABEZADOS

#include "Encabezados/stdafx.h"

#pragma endregion

#pragma region GENERAL

#include "General/Camera.h"
#include "General/Object.h"
#include "General/Player.h"

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


// --OTHER INCLUDES--

class Game
{
public: 
	// Public Attributes

	const char* name = "Nombre juego";

	// Constructors

	Game() :m_Window(NULL), m_renderer(NULL), m_Camera(NULL), m_dynamicsWorld(NULL), m_Player(NULL), m_sinTime(0) { m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f)); InitializeWindow(); };

	// Destructors

	~Game();
	
	// Public Methods
	int Start();	

	GLFWwindow* GetWindow() { return m_Window; }
	Camera* GetCamera() { return m_Camera; }

	void ProcessInput(GLFWwindow* window, int key, int action);
	void img_loader();
	
private:
	/// Private Attributes
	
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

	//// Private Methods

	/// Animations
	void performJumpAndSpin(std::string id, float time1, float time2);
	//Time
	float m_sinTime;
	// Constants for animation parameters
	const float jumpHeight = 0.3f;
	const float jumpDuration = 2.0f;

	const float spinSpeed = 2.0f;



	// Initializers
	void StartGame();
	void InitializePhysics();
	void InitializeGraphics();
	int InitializeWindow();

	// Functionality
	void Run();
	
	void Actualizar(float deltaTime);
	void Render();

	void LoadScreen();
};