#include "PlayerVehicle.h"

// Constructors

PlayerVehicle::PlayerVehicle(btDiscreteDynamicsWorld* dynamicsWorld) {
	initVehicle(dynamicsWorld);
}

PlayerVehicle::PlayerVehicle(btDiscreteDynamicsWorld* dynamicsWorld, int mass, int suspensionRestLength, int wheelWidth, int wheelRadius, int connectionHeight) :
	vehicleParams(mass, suspensionRestLength, wheelWidth, wheelRadius, connectionHeight)
{
	btScalar m_mass = mass;
	btScalar m_suspensionRestLength = suspensionRestLength;
	btScalar m_wheelWidth = wheelWidth;
	btScalar m_wheelRadius = wheelRadius;
	btScalar m_connectionHeight = connectionHeight;	//The height where the wheels are connected to the chassis

	// On The Road params
	btScalar m_fEngineForce = 5000;
	btScalar m_bEngineForce = 3000;
	btScalar m_steeringValue = 0.3;

	initVehicle(dynamicsWorld);
}

// Destructor

PlayerVehicle::~PlayerVehicle() {
	exitVehicle();
}

// Public Callbacks

bool PlayerVehicle::keyboardCallback(bool* keys, GLfloat deltaTime, int state) {
	bool handled = false;

	// 0 => rueda delantera
	// 1 => rueda delantera
	// 2 => rueda trasera
	// 3 => rueda trasera

	// state indica si se ha pulsado o soltado
	if (state == GLFW_PRESS) {
		if (keys[GLFW_KEY_W])
		{
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce * deltaTime, 2); 
			this->vehicle->applyEngineForce(this->vehicleParams.m_fEngineForce * deltaTime, 3);
			handled = true;
		}
		if (keys[GLFW_KEY_S])
		{
			this->vehicle->applyEngineForce(this->vehicleParams.m_bEngineForce * deltaTime, 0);
			this->vehicle->applyEngineForce(this->vehicleParams.m_bEngineForce * deltaTime, 1);
			handled = true;
		}
		if (keys[GLFW_KEY_A])
		{
			this->vehicle->setSteeringValue(this->vehicleParams.m_steeringValue, 0);
			this->vehicle->setSteeringValue(this->vehicleParams.m_steeringValue, 1);
			handled = true;

		}
		if (keys[GLFW_KEY_D])
		{
			this->vehicle->setSteeringValue(-this->vehicleParams.m_steeringValue, 0);
			this->vehicle->setSteeringValue(-this->vehicleParams.m_steeringValue, 1);
			handled = true;
		}
		// Handbrake
		if (keys[GLFW_KEY_LEFT_CONTROL]) {
			this->vehicle->setBrake(500 * deltaTime, 2);
			this->vehicle->setBrake(500 * deltaTime, 3);
			handled = true;
		}
	}
	else {
		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_S])
		{
			this->vehicle->applyEngineForce(0, 2);
			this->vehicle->applyEngineForce(0, 3);

			//Default braking force, always added otherwise there is no friction on the wheels
			this->vehicle->setBrake(10, 2);
			this->vehicle->setBrake(10, 3);
			handled = true;
		}

		if (keys[GLFW_KEY_A] || keys[GLFW_KEY_D])
		{
			this->vehicle->setSteeringValue(0, 0);
			this->vehicle->setSteeringValue(0, 1);
			handled = true;
		}

		if (keys[GLFW_KEY_LEFT_CONTROL])
		{
			this->vehicle->setBrake(0, 2);
			this->vehicle->setBrake(0, 3);
			handled = true;
		}
	}

	return handled;
}

// Public Methods

void PlayerVehicle::renderScene() {
	// ??
}

void PlayerVehicle::physicsDebugDraw(int debugFlags) {
	// ??
}

// Private Methods

int PlayerVehicle::initVehicle(btDiscreteDynamicsWorld* dynamicsWorld) {
	if (!dynamicsWorld) { printf("Use initialized btDiscreteDynamicsWorld!!"); return -1; }

	{
		//the dimensions for the boxShape are half extents, so 0,5 equals to 
		btVector3 halfExtends(1, btScalar(0.5), 2);

		//The btBoxShape is centered at the origin
		btCollisionShape* chassisShape = new btBoxShape(halfExtends);

		//Stores on an array for reusing
		this->collisionShapes.push_back(chassisShape);

		//A compound shape is used so we can easily shift the center of gravity of our vehicle to its bottom
		//This is needed to make our vehicle more stable
		btCompoundShape* compound = new btCompoundShape();

		this->collisionShapes.push_back(compound);

		btTransform localTransform;
		localTransform.setIdentity();
		localTransform.setOrigin(btVector3(0, 1, 0)); // This moves the center of mass, not the vehicle position, do not set too far away from the origin

		//The center of gravity of the compound shape is the origin. When we add a rigidbody to the compound shape
		//it's center of gravity does not change. This way we can add the chassis rigidbody one unit above our center of gravity
		//keeping it under our chassis, and not in the middle of it
		compound->addChildShape(localTransform, chassisShape);

		//Creates a rigid body
		btRigidBody* chassisRigidBody = this->createChassisRigidBodyFromShape(compound);

		//Adds the vehicle chassis to the world
		dynamicsWorld->addRigidBody(chassisRigidBody);

		btVehicleRaycaster* vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld);

		btRaycastVehicle::btVehicleTuning tuning;

		//Creates a new instance of the raycast vehicle
		this->vehicle = new btRaycastVehicle(tuning, chassisRigidBody, vehicleRayCaster);

		//Never deactivate the vehicle
		chassisRigidBody->setActivationState(DISABLE_DEACTIVATION);

		//Adds the vehicle to the world
		dynamicsWorld->addVehicle(this->vehicle);

		//Adds the wheels to the vehicle
		this->addWheels(&halfExtends, this->vehicle, tuning);
	}

	// guiHelper->autogenerateGraphicsObjects(this->dynamicsWorld);
}

void PlayerVehicle::exitVehicle() {

	//delete collision shapes
	for (int j = 0; j < this->collisionShapes.size(); j++)
	{
		btCollisionShape* shape = this->collisionShapes[j];
		delete shape;
	}
	this->collisionShapes.clear();

	delete this->vehicle;
}

btRigidBody* PlayerVehicle::createChassisRigidBodyFromShape(btCollisionShape* chassisShape) {
	btTransform chassisTransform;
	chassisTransform.setIdentity();
	chassisTransform.setOrigin(btVector3(10, 1, 10)); // This changes the car starting position

	{
		//chassis mass 
		btScalar mass(this->vehicleParams.m_mass);

		//since it is dynamic, we calculate its local inertia
		btVector3 localInertia(0, 0, 0);
		chassisShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* chassisMotionState = new btDefaultMotionState(chassisTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, chassisMotionState, chassisShape, localInertia);

		return new btRigidBody(rbInfo);
	}
}

void PlayerVehicle::addWheels(btVector3* halfExtents, btRaycastVehicle* vehicle, btRaycastVehicle::btVehicleTuning tuning) {
	//The direction of the raycast, the btRaycastVehicle uses raycasts instead of simulating the wheels with rigid bodies
	btVector3 wheelDirectionCS0(0, -1, 0);

	//The axis which the wheel rotates arround
	btVector3 wheelAxleCS(-1, 0, 0);

	btScalar suspensionRestLength(this->vehicleParams.m_suspensionRestLength);

	btScalar wheelWidth(this->vehicleParams.m_wheelWidth);

	btScalar wheelRadius(this->vehicleParams.m_wheelRadius);

	//The height where the wheels are connected to the chassis
	btScalar connectionHeight(this->vehicleParams.m_connectionHeight);

	//All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
	btVector3 wheelConnectionPoint(halfExtents->x() - wheelRadius, connectionHeight, halfExtents->z() - wheelWidth);

	//Adds the front wheels
	vehicle->addWheel(wheelConnectionPoint, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	//Adds the rear wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3(1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	//Configures each wheel of our vehicle, setting its friction, damping compression, etc.
	//For more details on what each parameter does, refer to the docs
	for (int i = 0; i < vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
	}
}

