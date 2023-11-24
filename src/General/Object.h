#pragma once

// --OUR INCLUDES--
#include "../Encabezados/stdafx.h"

#pragma region Graficos

#include "../Graficos/Modelo/Model.h"

#pragma endregion

// --OTHER INCLUDES--

#include "btBulletDynamicsCommon.h"

class Object {
public:

	// Public Attributes
	Model* model;
	btRigidBody* rb;

	// Constructor
	Object(std::string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld);

	// Public Methods
	void Draw(Shader& shader);

private:

	// Private Methods
	void CreateRigidBody();
};