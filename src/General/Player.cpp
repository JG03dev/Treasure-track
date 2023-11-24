#include "Player.h"

// Constructor

Player::Player(string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld) {
	model = new Model(modelPath);

	this->CreateVehicle(modelPath, dynamicsWorld); // Doesn't use a compound shape as a test to see how it works

	dynamicsWorld->addVehicle(this->vehicle);
}

// Public Methods

void Player::Draw(Shader& shader) {
	model->Draw(shader);
}

void Player::InputMethod(int key, bool keyPressed) {
	// state indica si se ha pulsado o soltado

	if (keyPressed) {
		switch (key)
		{
		case GLFW_KEY_W:
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce, 2);
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce, 3);
			break;
		case GLFW_KEY_S:
			this->vehicle->applyEngineForce(-this->vehicleParams.m_bEngineForce, 0);
			this->vehicle->applyEngineForce(-this->vehicleParams.m_bEngineForce, 1);
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
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (key)
		{
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			this->vehicle->applyEngineForce(0, 2);
			this->vehicle->applyEngineForce(0, 3);

			//Default braking force, always added otherwise there is no friction on the wheels
			this->vehicle->setBrake(10, 2);
			this->vehicle->setBrake(10, 3);
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
	}
}

// Private Methods

void Player::CreateVehicle(string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld) {
	// --------- Mesh Load to Get the chassis half extents for the box shape

	btCompoundShape* tempShape = new btCompoundShape();

	btTransform chassisTransform;
	chassisTransform.setIdentity();
	chassisTransform.setOrigin(btVector3(0, 0, 0));

	for (int i = 0, numModels = model->meshes.size(); i < numModels; i++) {
		btTriangleMesh* triangleMesh = new btTriangleMesh();

		Mesh* mesh = &this->model->meshes.front();
		for (int i = 0; i < mesh->indices.size(); i += 3) {
			btVector3 v1(mesh->vertices[i].Position.x, mesh->vertices[i].Position.y, mesh->vertices[i].Position.z);
			btVector3 v2(mesh->vertices[i + 1].Position.x, mesh->vertices[i + 1].Position.y, mesh->vertices[i + 1].Position.z);
			btVector3 v3(mesh->vertices[i + 2].Position.x, mesh->vertices[i + 2].Position.y, mesh->vertices[i + 2].Position.z);

			triangleMesh->addTriangle(v1, v2, v3);
		}

		btConvexShape* convexShape = new btConvexTriangleMeshShape(triangleMesh);
		tempShape->addChildShape(chassisTransform, convexShape);
	}

	btVector3 min, max;

	tempShape->getAabb(btTransform::getIdentity(), min, max);
	btVector3 halfExtents = (max - min) / 2;

	// --------- Chassis RigidBody

	btCollisionShape* chassisShape = new btBoxShape(halfExtents);

	btScalar mass(this->vehicleParams.m_mass);

	btVector3 localInertia(0, 0, 0);
	chassisShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* chassisMotionState = new btDefaultMotionState(chassisTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, chassisMotionState, chassisShape, localInertia);

	btRigidBody* chassisRigidBody = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(chassisRigidBody);

	// --------- Raycast Vehicle

	btVehicleRaycaster* vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld);

	btRaycastVehicle::btVehicleTuning tuning;
	// tuning.m_suspensionStiffness
	// tuning.m_suspensionDamping
	// tuning.m_suspensionCompression
	// tuning.m_maxSuspensionForce
	// tuning.m_frictionSlip
	// tuning.m_maxSuspensionTravelCm0

	//Creates a new instance of the raycast vehicle
	this->vehicle = new btRaycastVehicle(tuning, chassisRigidBody, vehicleRayCaster);

	// --------- Add Wheels

	btVector3 wheelDirectionCS0(0, -1, 0);

	// The axis which the wheel rotates arround
	btVector3 wheelAxleCS(-1, 0, 0);

	btScalar suspensionRestLength(this->vehicleParams.m_suspensionRestLength);

	btScalar wheelWidth(this->vehicleParams.m_wheelWidth);

	btScalar wheelRadius(this->vehicleParams.m_wheelRadius);

	// The height where the wheels are connected to the chassis
	btScalar connectionHeight(this->vehicleParams.m_connectionHeight);

	// All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
	btVector3 wheelConnectionPoint(halfExtents.x() - wheelRadius, connectionHeight, halfExtents.z() - wheelWidth);

	// Adds the front wheels
	vehicle->addWheel(wheelConnectionPoint, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	// Adds the rear wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3(1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	// Configures each wheel of our vehicle, setting its friction, damping compression, etc.
	// For more details on what each parameter does, refer to the docs
	for (int i = 0; i < vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
	}

	this->vehicle->setCoordinateSystem(0, 1, 2); // To adjust the coordinate system to the OpenGL coordinate system
}