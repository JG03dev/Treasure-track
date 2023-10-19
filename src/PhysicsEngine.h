#pragma once

#include "btBulletDynamicsCommon.h"
#include "PlayerVehicle.h"

#include "Mesh.h"

#include "GroundMesh.h"

#include "Renderer.h"

class PhysicsEngine {

public:
	
	// Constructors

	PhysicsEngine();

	PhysicsEngine(float* gravity, int simulationSubSteps = 2);

	// Destructors

	~PhysicsEngine();

	// Public Methods
	
	void stepSimulation(float deltaTime);

	// Public Attributes

	btRigidBody* groundRB;

	PlayerVehicle* player;

private:

	// Private Methods

	void initPhysics();

	void exitPhysics();

	btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);

	// Private Attributes

	int simulationSubSteps;

	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	btVector3 gravity;

	Renderer* renderer;
};