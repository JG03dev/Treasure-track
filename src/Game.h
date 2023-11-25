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

#include "Graficos/Renderer.h"
#include "Graficos/Skybox.h"
#include "Graficos/Luces/Light.h"

#pragma endregion

// --OTHER INCLUDES--


class Game
{
public: 
	// Public Attributes

	const char* name = "Nombre juego";

	// Constructors

	Game() {};
	
	// Public Methods

	void StartGame();

	void AddShader(const char* s1, const char* s2);

	// Destructors

	~Game() {};

private:
	// Private Attributes
	
	// Window
	GLFWwindow* m_Window;
	const unsigned int m_SCR_WIDTH = 800;
	const unsigned int m_SCR_HEIGHT = 600;

	// Camera
	Camera* m_Camera;

	// Skybox
	Skybox m_skybox;

	// Graphics
	Renderer m_renderer;
	// These next 3 attributes will probably be stored in the Renderer
	Light* m_Light;
	glm::vec3 m_lightPos;
	std::vector<Shader*> m_Shaders;

	// Physics logic
	float m_deltaTime;
	float m_lastFrame;
	btDiscreteDynamicsWorld* m_dynamicsWorld;

	// Objects
	Player* m_Player;
	std::vector<Object*> m_Objects;

	// Not used
	bool m_blinn;
	bool m_blinnKeyPressed;
	bool m_firstMouse;
	float m_lastX;
	float m_lastY;

	// Private Methods

	// Initializers
	void Initializer();
	void InitializePhysics();
	int InitializeWindow();
	Skybox loadSkybox();

	// Functionality
	void Run();
	void ProcessInput(GLFWwindow* window);
	void Actualizar();
	void Render();
};