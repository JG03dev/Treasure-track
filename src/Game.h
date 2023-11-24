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

#include "Graficos/Skybox.h"
#include "Graficos/Luces/Light.h"

#pragma endregion

// --OTHER INCLUDES--


class Game
{
public: 
	const char* name = "Nombre juego";
	const GLint windowHeight = 0; //Alto
	const GLint windowWeight = 0; //Ancho


	Game() = default;
	~Game() = default;

	Game(const Game& other) = delete;
	Game& operator==(const Game& other) = delete;

	Game(GLFWwindow* w) : m_Window(w) {};


	void Initializer();
	void Run();
	void processInput(GLFWwindow* window);
	void Actualizar();
	void Render();

	int InitializeWindow();

	Skybox loadSkybox();

	void AddShader(const char* s1, const char* s2);

	//physics
	void InitializePhysics();

private:
	GLFWwindow* m_Window; 
	Camera* m_Camera;
	Light* m_Light;

	Skybox m_skybox;

	const unsigned int m_SCR_WIDTH = 800;
	const unsigned int m_SCR_HEIGHT = 600;

	float m_deltaTime;
	float m_lastFrame;

	bool m_blinn;
	bool m_blinnKeyPressed;

	bool m_firstMouse;

	float m_lastX;
	float m_lastY;

	glm::vec3 m_lightPos; 

	std::vector<Shader*> m_Shaders;
	Player* m_Player;
	std::vector<Object*> m_Objects;

	btDiscreteDynamicsWorld* m_dynamicsWorld;
};