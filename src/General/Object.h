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
	Object(Model* m, btDiscreteDynamicsWorld* dynamicsWorld, glm::mat4 initOpenGLMatrix);

	// Destructor (First call DestroyObject, then delete object)
	void DestroyObject(btDiscreteDynamicsWorld* dynamicsWorld);
	~Object() {} // Destructor managed

	// Public Methods
	void setOpenGLMatrixToPhysics(const glm::mat4 openGLMatrix);
private:

	// Private Attributes
	// Memory Management
	btAlignedObjectArray<btCollisionShape*> m_CollisionShapes;

	// Private Methods
	void CreateRigidBody(Model& hitbox);

	
};