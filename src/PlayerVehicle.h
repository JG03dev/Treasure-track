#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "btBulletDynamicsCommon.h"
//#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
//#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
//#include "BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h"
//#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"
//#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
//#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
//#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
//#include "LinearMath/btAlignedObjectArray.h"
//#include "btBulletCollisionCommon.h"
//#include <stdio.h>

class PlayerVehicle {
public:
	// Constructors

	PlayerVehicle(btDiscreteDynamicsWorld* dynamicsWorld);

	PlayerVehicle(btDiscreteDynamicsWorld* dynamicsWorld, int mass, int suspensionRestLength, int wheelWidth, int wheelRadius, int connectionHeight);

	// Destructors
	~PlayerVehicle();

	// Public Callbacks
	bool mouseMoveCallback(float x, float y)
	{
		return false; // ??
	}

	bool mouseButtonCallback(int button, int state, float x, float y)
	{
		return false; // ??
	}

	bool keyboardCallback(bool* keys, GLfloat deltaTime, int state);

	// Public Methods

	void renderScene();

	void physicsDebugDraw(int debugFlags);

	void resetCamera()
	{
		float dist = 5 * 8;
		float pitch = -45;
		float yaw = 32;
		float targetPos[3] = { -0.33, -0.72, 4.5 };
		//guiHelper->resetCamera(dist, pitch, yaw, targetPos[0], targetPos[1], targetPos[2]);
	}

	// Public Attributes

	btRaycastVehicle* vehicle;	// Main attribute, this stores the vehicle

	struct VehicleParams {
		// Construction Params
		btScalar m_mass = 1200;
		btScalar m_suspensionRestLength = 0.7;
		btScalar m_wheelWidth = 0.4;
		btScalar m_wheelRadius = 0.5;
		btScalar m_connectionHeight = 1.2;	//The height where the wheels are connected to the chassis

		// On The Road params
		btScalar m_fEngineForce = 5000;
		btScalar m_bEngineForce = 3000;
		btScalar m_steeringValue = 0.3;

		VehicleParams() : m_mass(1200), m_suspensionRestLength(0.7), m_wheelWidth(0.4),	m_wheelRadius(0.5),	m_connectionHeight(1.2) {}

		VehicleParams(float mass, float suspensionRestLengt, float wheelWidth, float wheelRadius, float connectionHeight) :
			m_mass(mass), 
			m_suspensionRestLength(suspensionRestLengt), 
			m_wheelWidth(wheelWidth),
			m_wheelRadius(wheelRadius),
			m_connectionHeight(connectionHeight) {}

	} vehicleParams;

	struct WheelParams {
		btScalar m_suspensionStiffness = 50;
		btScalar m_frictionSlip = 1.2;	//Larger friction slips will result in better handling
		btScalar m_rollInfluence = 1;
	} wheelParams;

private:
	// Private Methods

	int initVehicle(btDiscreteDynamicsWorld* dynamicsWorld);

	void exitVehicle();

	btRigidBody* createChassisRigidBodyFromShape(btCollisionShape* chassisShape);

	void addWheels(btVector3* halfExtents, btRaycastVehicle* vehicle, btRaycastVehicle::btVehicleTuning tuning);

	// Private Attributes
	
	// GUIHelperInterface* guiHelper;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;	// Array of shapes used for the vehicle. For reuse and memory cleaning purposes
};