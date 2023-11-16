#pragma once

#include <memory>

#include "stdafx.h"

#include "skybox.h"
#include "Light.h"
#include "Camera.h"
#include "Object.h"
#include "Window.h"

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

	void Run();
	void ManejarEntradaDeUsuario();
	void Actualizar();
	void Renderizar();

private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Light> m_Light;
	std::unique_ptr<Car> m_Car;
};

//classe pel cotxe principal
class Car
{
public:
	Object skin;
	GLfloat getX() { return posX; }
	GLfloat getY() { return posX; }

	void changeVelocity(GLfloat vel) { velocity = vel; }
	void actualizarPos();

private:
	GLfloat posX;
	GLfloat posY;

	GLfloat velocity;

};

//classe pels cotxes extra que si els toquem resten punts
class Extra
{

};