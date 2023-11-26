#pragma once

// --OUR INCLUDES--

#pragma region MODELO

#include "../Graficos/Modelo/Model.h"

#pragma endregion

// --OTHER INCLUDES--

#include "btBulletDynamicsCommon.h"
#include <GLFW/glfw3.h>

class Player {
public:

	// Public Attributes
	Model* model;
	btRaycastVehicle* vehicle;

	// Constructor
	Player(std::string const& modelPath, std::string const& modelName, btDiscreteDynamicsWorld* dynamicsWorld, GLfloat sIntensity, GLfloat shine);

	// Destructor
	~Player();

	// Public Methods
	void Draw(Shader& shader);
	void InputMethod(int key, int keyPressed);

private:
	// Private Attributes

	struct VehicleParams {
		// Construction Params
		btScalar m_mass = 1200;
		btScalar m_suspensionRestLength = 0.5;
		btScalar m_wheelWidth = 0.4;
		btScalar m_wheelRadius = 0.5;
		btScalar m_connectionHeight = 1.2;	//The height where the wheels are connected to the chassis

		// On The Road params
		btScalar m_fEngineForce = 5000;
		btScalar m_bEngineForce = 3000;
		btScalar m_steeringValue = 0.3;
	} vehicleParams;

	struct WheelParams {
		btScalar m_suspensionStiffness = 5.88; // 5.88;
		btScalar m_suspensionCompression = 0.83; //0.83;
		btScalar m_suspensionDamping = 0.88; //0.88;
		btScalar m_maxSuspensionTravelCm = 500.;
		btScalar m_frictionSlip = 10.5;//10.5;	//Larger friction slips will result in better handling
		btScalar m_maxSuspensionForce = 6000;//6000.;
		//btScalar m_rollInfluence = 1;
	} wheelParams;

	// Private Methods

	void CreateVehicle(std::string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld);
};