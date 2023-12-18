#pragma once

#include "../Encabezados/stdafx.h"
#include "btBulletDynamicsCommon.h"
#include "../Graficos/Modelo/Model.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class Coin {
public:
	// Public Attributes
	Model* model;
	std::string m_id;
	
    // Constructors
	Coin(Model* m, btDiscreteDynamicsWorld* dynamicsWorld, glm::mat4 initOpenGLMatrix, std::string const& id);

	// Destructors
	void DestroyObject(btDiscreteDynamicsWorld* dynamicsWorld);
	~Coin() {}

	//Getters

    btPairCachingGhostObject* getGhostObject() { return ghostObject; }

private:
	
	// Private Attributes
	// Memory Management
	btAlignedObjectArray<btCollisionShape*> m_CollisionShapes;
	btPairCachingGhostObject* ghostObject;
	btGhostPairCallback* m_GhostPairCallback;

    void CreateGhostObject(Model& hitbox);
    void setOpenGLMatrixToPhysics(const glm::mat4 openGLMatrix);
};