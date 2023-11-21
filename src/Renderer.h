#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Window.h"

class Renderer {
public:

	// Constructors

	Renderer() : uniformProjection(0), uniformModel(0), uniformView(0) {};

	Renderer(const Renderer& r) = delete;

	// Singleton
	static Renderer* getInstance() {
		if (!renderer) { renderer = new Renderer(); }
		return renderer;
	}

	// Public Methods

	void startWindow(GLint windowWidth = 1280, GLint windowHeight = 720) { 
		mainWindow = new Window(windowWidth, windowHeight); 
		mainWindow->Initialise(); 
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow->getBufferWidth() / (GLfloat)mainWindow->getBufferHeight(), 0.1f, 100.0f);
	}

	void addMesh(Mesh* m) { meshList.push_back(m); }

	void addShader(Shader* s) { shaderList.push_back(s); }

	void addCamera(Camera* c) { this->camera = c; }

	void updateCameraPos() { this->camera->followPlayer(); }

	void updateCameraRotation() { this->camera->mouseControl(this->mainWindow->getXChange(), this->mainWindow->getYChange()); }

	void clearWindow() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void render(btRigidBody* groundRB, btTransform chassisT);

	bool* getKeys() { return mainWindow->getKeys(); }

	// Destructors

	~Renderer() {};

	// Public Attributes

	glm::mat4 projection;

	Window* mainWindow;

private:

	// Singleton Instance
	
	static Renderer* renderer; // Singleton to access renderer class from anywhere

	// Private Attributes

	std::vector<Mesh*> meshList;

	std::vector<Shader*> shaderList;

	Camera* camera;

	PlayerVehicle* player;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
};