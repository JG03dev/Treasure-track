#include "Player.h"

// Constructor

// TODO: I would delete this constructor since it would lead to memory leaks
Player::Player(std::string const& modelPath, std::string const& modelName, btDiscreteDynamicsWorld* dynamicsWorld, GLfloat sIntensity, GLfloat shine) {
	model = new Model(sIntensity, shine);
	model->LoadModel(modelPath, modelName);

	this->CreateVehicle(dynamicsWorld); // Doesn't use a compound shape as a test to see how it works

	dynamicsWorld->addVehicle(this->vehicle);
}

Player::Player(Model* m, btDiscreteDynamicsWorld* dynamicsWorld)
{
	model = m; // Model points to the same model shared

	this->CreateVehicle(dynamicsWorld); // Doesn't use a compound shape as a test to see how it works

	dynamicsWorld->addVehicle(this->vehicle);
}

// Destructor
Player::~Player() {
	delete model;
}

// Public Methods

void Player::Draw(Shader& shader) {
	//model->Draw(shader);
}

void Player::InputMethod(int key, int keyPressed) {
	// state indica si se ha pulsado o soltado

	if (keyPressed == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_W:
			this->vehicle->setBrake(0, 0);
			this->vehicle->setBrake(0, 1);
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce, 2);
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce, 3);
			break;
		case GLFW_KEY_S:
			this->vehicle->applyEngineForce(-this->vehicleParams.m_bEngineForce, 0);
			this->vehicle->applyEngineForce(-this->vehicleParams.m_bEngineForce, 1);
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			break;
		case GLFW_KEY_A:
			this->vehicle->setSteeringValue(this->vehicleParams.m_steeringValue, 0);
			this->vehicle->setSteeringValue(this->vehicleParams.m_steeringValue, 1);
			break;
		case GLFW_KEY_D:
			this->vehicle->setSteeringValue(-this->vehicleParams.m_steeringValue, 0);
			this->vehicle->setSteeringValue(-this->vehicleParams.m_steeringValue, 1);
			break;
		case GLFW_KEY_LEFT_CONTROL:
			this->vehicle->setBrake(150, 2);
			this->vehicle->setBrake(150, 3);
			break;
		default:
			break;
		}
		return;
	}
	if (keyPressed == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			this->vehicle->applyEngineForce(0, 0);
			this->vehicle->applyEngineForce(0, 1);
			this->vehicle->applyEngineForce(0, 2);
			this->vehicle->applyEngineForce(0, 3);

			//Default braking force, always added otherwise there is no friction on the wheels
			this->vehicle->setBrake(50, 2);
			this->vehicle->setBrake(50, 3);
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			this->vehicle->setSteeringValue(0, 0);
			this->vehicle->setSteeringValue(0, 1);
			break;
		case GLFW_KEY_LEFT_CONTROL:
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			break;
		default:
			break;
		}
		return;
	}
}

// Private Methods

void Player::CreateVehicle(btDiscreteDynamicsWorld* dynamicsWorld) {
	// --------- Mesh Load to Get the chassis half extents for the box shape

	btCompoundShape tempShape;

	btTransform chassisTransform;
	chassisTransform.setIdentity();
	chassisTransform.setOrigin(btVector3(0, -2.8, 0));

	for (int i = 0, numModels = model->meshList.size(); i < numModels; i++) {
		btConvexHullShape convexShape;

		//TODO: Adaptar a nuevo mesh [DONE?]
		Mesh* mesh = this->model->meshList[i];
		const std::vector<GLfloat>* vertices = mesh->GetVertices();
		for (int j = 0; j < vertices->size(); j += 8) {
			btVector3 v1((*vertices)[j], (*vertices)[j + 1], (*vertices)[j + 2]);

			convexShape.addPoint(v1);
		}

		tempShape.addChildShape(chassisTransform, &convexShape);
	}

	btVector3 min, max;

	tempShape.getAabb(btTransform::getIdentity(), min, max);
	btVector3 halfExtents = (max - min) / 2;

	// --------- Chassis RigidBody

	btCollisionShape* chassisShape = new btBoxShape(halfExtents);
	chassisShape->setMargin(0.01f);

	btScalar mass(this->vehicleParams.m_mass);

	btVector3 localInertia(0, 0, 0);
	chassisShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* chassisMotionState = new btDefaultMotionState(chassisTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, chassisMotionState, chassisShape, localInertia);

	btRigidBody* chassisRigidBody = new btRigidBody(rbInfo);
	chassisRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(chassisRigidBody);

	// --------- Raycast Vehicle

	btVehicleRaycaster* vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld);

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_suspensionStiffness = wheelParams.m_suspensionStiffness;
	tuning.m_suspensionCompression = wheelParams.m_suspensionCompression;
	tuning.m_suspensionDamping = wheelParams.m_suspensionDamping;
	tuning.m_maxSuspensionTravelCm = wheelParams.m_maxSuspensionTravelCm;
	tuning.m_frictionSlip = wheelParams.m_frictionSlip;
	tuning.m_maxSuspensionForce = wheelParams.m_maxSuspensionForce;

	//Creates a new instance of the raycast vehicle
	this->vehicle = new btRaycastVehicle(tuning, chassisRigidBody, vehicleRayCaster);

	this->vehicle->setCoordinateSystem(2, 1, 0); // To adjust the coordinate system to the OpenGL coordinate system

	// --------- Add Wheels

	btVector3 wheelDirectionCS0(0, -1, 0);

	// The axis which the wheel rotates arround
	btVector3 wheelAxleCS(0, 0, 1);

	btScalar suspensionRestLength(this->vehicleParams.m_suspensionRestLength);

	btScalar wheelWidth(this->vehicleParams.m_wheelWidth);

	btScalar wheelRadius(this->vehicleParams.m_wheelRadius);

	// The height where the wheels are connected to the chassis
	btScalar connectionHeight(this->vehicleParams.m_connectionHeight);

	// All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
	btVector3 wheelConnectionPoint(halfExtents.x() - wheelRadius, connectionHeight, halfExtents.z() - wheelWidth);

	// Adds the front wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3(1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	vehicle->addWheel(wheelConnectionPoint * btVector3(1, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	// Adds the rear wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	// Configures each wheel of our vehicle, setting its friction, damping compression, etc.
	// For more details on what each parameter does, refer to the docs
	for (int i = 0; i < vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
	}
}