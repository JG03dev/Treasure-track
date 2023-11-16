#include "Game.h"

void Game::Run()
{
	m_Window = std::make_unique<Window>(windowHeight, windowWeight);

	while (m_Window->Initialise())
	{
		//Manejar entrada de usuario
		ManejarEntradaDeUsuario();

		//Actualizar Informacion (Mover coordenadas)
		Actualizar();

		//Renderizar
		Renderizar();
		
	}
}

void Game::ManejarEntradaDeUsuario()
{
	//m_Window->RecibirEventos();

	//aqui van los movimientos
}

void Game::Actualizar()
{
	//actualizar las coordenadas
}

void Game::Renderizar()
{
	//dibujar
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);

	/*
	m_Window->Cambiar(); 
	
	donde:
	void Window::Cambiar()
	{
		SDL_GL_SwapWindow((SDL_Window*)m_WindowHandle);
	}
	*/
}