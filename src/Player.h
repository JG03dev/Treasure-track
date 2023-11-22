#pragma once

#include "Model.h"
#include "btBulletDynamicsCommon.h"

class Player {
public:
	// ATTENTION:
	// ONLY 1 MESH IS USED FOR THIS CLASS RB
	// MORE WILL BE ADDED AS SOON AS I TEST THIS WORKS
	// AND WILL PROBABLY MAKE A RB WITH A COMPOUND SHAPE COLLISION SHAPE

	// Public Attributes
	Model* model;
	btRaycastVehicle* vehicle;

	// Constructor
	Player(string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld);

	// void InputMethod();?

private:
	// Private Attributes

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
	} vehicleParams;

	struct WheelParams {
		btScalar m_suspensionStiffness = 50;
		btScalar m_frictionSlip = 1.2;	//Larger friction slips will result in better handling
		btScalar m_rollInfluence = 1;
	} wheelParams;

	// Private Methods

	void CreateVehicle(string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld);
};