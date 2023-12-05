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
	Coin(Model* m, btDiscreteDynamicsWorld* dynamicsWorld, glm::mat4 initOpenGLMatrix, std::string const& id) : m_id(id)
	{
		model = m; // Model points to the same model shared

		Model hitbox;
		hitbox.LoadModel(std::string("../../../Assets/") + m->GetName() + std::string("/") + m->GetName() + std::string("Hitbox.obj"), m->GetName() + std::string("Hitbox"));
		this->CreateGhostObject(hitbox);

		setOpenGLMatrixToPhysics(initOpenGLMatrix);

		dynamicsWorld->addCollisionObject(this->ghostObject);
		dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	}

    
    ~Coin() {
        delete ghostObject;
    }

    btPairCachingGhostObject* getGhostObject() {
        return ghostObject;
    }

private:
	
	btPairCachingGhostObject* ghostObject;

    void CreateGhostObject(Model& hitbox);
    void setOpenGLMatrixToPhysics(const glm::mat4 openGLMatrix);
};