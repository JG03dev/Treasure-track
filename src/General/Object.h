#pragma once

// --OUR INCLUDES--

#pragma region Graficos

#include "../Graficos/Modelo/Model.h"

#pragma endregion

// --OTHER INCLUDES--

#include "btBulletDynamicsCommon.h"

class Object {
public:
	// ATTENTION:
	// ONLY 1 MESH IS USED FOR THIS CLASS RB
	// MORE WILL BE ADDED AS SOON AS I TEST THIS WORKS
	// AND WILL PROBABLY MAKE A RB WITH A COMPOUND SHAPE COLLISION SHAPE

	// Public Attributes
	Model* model;
	btRigidBody* rb;

	// Constructor
	Object(string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld);

	// Public Methods
	void Draw(Shader& shader);

private:

	// Private Methods
	void CreateRigidBody();
};