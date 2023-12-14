#include "Player.h"

// Constructor
Player::Player(Model* m, btDiscreteDynamicsWorld* dynamicsWorld) : modelWheel(NULL), delaIzquierda(NULL), delaDerecha(NULL), m_num_keys(0)
{
	modelChasis = m; // Model points to the same model shared

	Model hitbox;
	hitbox.LoadModel(std::string("../../../Assets/") + m->GetName() + std::string("/") + m->GetName() + std::string("Hitbox.obj"), m->GetName() + std::string("Hitbox"));
	this->CreateVehicle(dynamicsWorld, hitbox);
	dynamicsWorld->addVehicle(this->vehicle);
	
	lastQuaternion = this->getCarRotation();
	lastForward = this->getCarForward();
}

// Destructor
Player::~Player() {
}

// Public Methods

void Player::updatePlayerData() //TODO: find more uses to this function
{
	glm::vec3 carPosition = getCarPos();
	glm::vec3 direction = getCarForward();
	glm::mat3x3 carBasis = getCarBasis();

	// Set the position of the lights relative to the car's local coordinates
	glm::vec3 positionLeft = carPosition + carBasis * glm::vec3(3.0f, 0.0f, 0.7f);
	glm::vec3 positionRight = carPosition + carBasis * glm::vec3(3.0f, 0.0f, -0.7f);

	if (delaIzquierda)
	{
		// Update light parameters
		delaIzquierda->SetFlash(positionLeft,direction);
	}

	if (delaDerecha)
	{
		// Update light parameters
		delaDerecha->SetFlash(positionRight, direction);
	}
}

void Player::Draw(Shader& shader) {
	//model->Draw(shader);
}

void Player::InputMethod(int key, int keyPressed) {
	// state indica si se ha pulsado o soltado

	if (keyPressed == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_W:
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce, 1);
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce, 0);
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			m_num_keys++;
			break;
		case GLFW_KEY_S:
			this->vehicle->applyEngineForce(-this->vehicleParams.m_bEngineForce, 2);
			this->vehicle->applyEngineForce(-this->vehicleParams.m_bEngineForce, 3);
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			m_num_keys++;
			break;
		case GLFW_KEY_A:
			this->vehicle->setSteeringValue(this->vehicleParams.m_steeringValue, 0);
			this->vehicle->setSteeringValue(this->vehicleParams.m_steeringValue, 1);
			break;
		case GLFW_KEY_D:
			this->vehicle->setSteeringValue(-this->vehicleParams.m_steeringValue, 0);
			this->vehicle->setSteeringValue(-this->vehicleParams.m_steeringValue, 1);
			break;
		case GLFW_KEY_SPACE:
			this->vehicle->setBrake(600, 2);
			this->vehicle->setBrake(600, 3);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce * 2, 2);
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce * 2, 3);
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			m_num_keys++;
			break;
		case GLFW_KEY_R:
		{
			for (int i = 0; i < 4; i++) {
				this->vehicle->applyEngineForce(0, i);
				this->vehicle->setSteeringValue(0, i);
			}
			btTransform t;
			t.setOrigin(btVector3(0, -3.2, 0));
			t.setRotation(btQuaternion(0, 0, 0));
			this->vehicle->getRigidBody()->setWorldTransform(t);
			this->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
			this->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
			break;
		}
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
			m_num_keys--;
			this->vehicle->applyEngineForce(0, 0);
			this->vehicle->applyEngineForce(0, 1);
			if (m_num_keys == 0) {
				this->vehicle->setBrake(50, 2);
				this->vehicle->setBrake(50, 3);
			}
			break;
		case GLFW_KEY_S:
			m_num_keys--;
			this->vehicle->applyEngineForce(0, 2);
			this->vehicle->applyEngineForce(0, 3);
			if (m_num_keys == 0) {
				this->vehicle->setBrake(50, 2);
				this->vehicle->setBrake(50, 3);
			}
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			this->vehicle->setSteeringValue(0, 0);
			this->vehicle->setSteeringValue(0, 1);
			break;
		case GLFW_KEY_SPACE:
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			m_num_keys--;
			this->vehicle->applyEngineForce(0, 2);
			this->vehicle->applyEngineForce(0, 3);
			if (m_num_keys == 0) {
				this->vehicle->setBrake(50, 2);
				this->vehicle->setBrake(50, 3);
			}
			break;
		default:
			break;
		}
		return;
	}
}

void Player::AddWheelModel(std::string const& modelPath, std::string const& modelName, GLfloat sIntensity, GLfloat shine) {
	modelWheel = new Model(sIntensity, shine);
	modelWheel->LoadModel(modelPath, modelName);
}

glm::mat3x3 Player::getCarBasis()
{
	btTransform carTransform = vehicle->getChassisWorldTransform();
	glm::vec3 carX(carTransform.getBasis().getColumn(0).x(), carTransform.getBasis().getColumn(0).y(), carTransform.getBasis().getColumn(0).z()); //first column is X
	glm::vec3 carY(carTransform.getBasis().getColumn(1).x(), carTransform.getBasis().getColumn(1).y(), carTransform.getBasis().getColumn(1).z()); //second column is Y
	glm::vec3 carZ(carTransform.getBasis().getColumn(2).x(), carTransform.getBasis().getColumn(2).y(), carTransform.getBasis().getColumn(2).z()); //last column is Z

	return glm::mat3x3(carX, carY, carZ);
}

glm::vec3 Player::getCarPos()
{
	btTransform carTransform = vehicle->getChassisWorldTransform();

	return glm::vec3(carTransform.getOrigin().x(), carTransform.getOrigin().y(), carTransform.getOrigin().z());;
}

glm::vec3 Player::getCarForward()
{
	return glm::vec3(vehicle->getForwardVector().x(), vehicle->getForwardVector().y(), vehicle->getForwardVector().z());
}

btQuaternion Player::getCarRotation() {
	return vehicle->getChassisWorldTransform().getRotation();
}

// Private Methods

void Player::CreateVehicle(btDiscreteDynamicsWorld* dynamicsWorld, Model& hitbox) {
	// --------- Mesh Load to Get the chassis half extents for the box shape
	btCompoundShape* temp = new btCompoundShape();

	btTransform chassisTransform;
	chassisTransform.setIdentity();
	chassisTransform.setOrigin(btVector3(0, 0.3, 0));

	btConvexHullShape convexShape;

	//TODO: Adaptar a nuevo mesh [DONE?]
	Mesh* mesh = hitbox.meshList[0];
	const std::vector<GLfloat>* vertices = mesh->GetVertices();
	for (int j = 0; j < vertices->size(); j += 8) {
		btVector3 v1((*vertices)[j], (*vertices)[j + 1], (*vertices)[j + 2]);

		convexShape.addPoint(v1);
	}

	btVector3 min, max;

	convexShape.getAabb(chassisTransform, min, max);
	btVector3 halfExtents = (max - min) / 2;
	
	// --------- Chassis RigidBody

	btCollisionShape* chassisShape = new btBoxShape(halfExtents);
	temp->addChildShape(chassisTransform, chassisShape);

	btScalar mass(this->vehicleParams.m_mass);

	btVector3 localInertia(0, 0, 0);
	chassisShape->calculateLocalInertia(mass, localInertia);
	chassisTransform.setOrigin(btVector3(0, -3.2, 0));

	btDefaultMotionState* chassisMotionState = new btDefaultMotionState(chassisTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, chassisMotionState, temp, localInertia);

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
	btVector3 wheelConnectionPoint(halfExtents.x() - wheelRadius*2, 0.6, halfExtents.z() - wheelWidth);

	// Adds the front wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3(1.2, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	vehicle->addWheel(wheelConnectionPoint * btVector3(1.2, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	// Adds the rear wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	// Configures each wheel of our vehicle, setting its friction, damping compression, etc.
	// For more details on what each parameter does, refer to the docs
	//for (int i = 0; i < vehicle->getNumWheels(); i++)
	//{
	//	btWheelInfo& wheel = vehicle->getWheelInfo(i);
	//	wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
	//	wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
	//}
}