#pragma once

#include "Model.h"
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

private:

	// Private Methods
	void CreateRigidBody();
};