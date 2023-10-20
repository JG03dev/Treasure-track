#include "PhysicsEngine.h"

// Constructors

PhysicsEngine::PhysicsEngine() : gravity(0, -9.8, 0), simulationSubSteps(2) {
	renderer = Renderer::getInstance();
	
	initPhysics();

	player = new PlayerVehicle(dynamicsWorld);

	
}

PhysicsEngine::PhysicsEngine(float* g, int simulationSubSteps) : gravity(btScalar(g[0]), btScalar(g[1]), btScalar(g[2])), simulationSubSteps(simulationSubSteps) {
	renderer = Renderer::getInstance();

	initPhysics();

	player = new PlayerVehicle(dynamicsWorld);
}

// Destructors

PhysicsEngine::~PhysicsEngine() {

}

// Public Methods

void PhysicsEngine::stepSimulation(float deltaTime) {
	dynamicsWorld->stepSimulation(deltaTime, simulationSubSteps);
}

// Private Methods

void PhysicsEngine::initPhysics() {
	//Collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	//Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	//The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//Creates the dynamics world, wich will be responsable for managing our physics objects and constraints
	this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	this->dynamicsWorld->setGravity(gravity);

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	{
		//Creates the ground shape
		btCollisionShape* groundShape = new btBoxShape(btVector3(10, 1, 10));

		Mesh* groundM = new Mesh();
		groundM->CreateMesh(gVerticesGround, gIndicesBunny, GROUND_NUM_VERTICES, GROUND_NUM_INDICES);
		renderer->addMesh(groundM);

		//Stores on an array for reusing
		this->collisionShapes.push_back(groundShape);

		//Creates the ground rigidbody
		groundRB = this->createGroundRigidBodyFromShape(groundShape);

		//Adds it to the world
		this->dynamicsWorld->addRigidBody(groundRB);
	}
}

void PhysicsEngine::exitPhysics() {
	for (int i = this->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = this->dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			while (body->getNumConstraintRefs())
			{
				btTypedConstraint* constraint = body->getConstraintRef(0);
				this->dynamicsWorld->removeConstraint(constraint);
				delete constraint;
			}
			delete body->getMotionState();
			this->dynamicsWorld->removeRigidBody(body);
		}
		else
		{
			this->dynamicsWorld->removeCollisionObject(obj);
		}
		delete obj;
	}

	for (int j = 0; j < this->collisionShapes.size(); j++)
	{
		btCollisionShape* shape = this->collisionShapes[j];
		delete shape;
	}
	collisionShapes.clear();

	btConstraintSolver* constraintSolver = this->dynamicsWorld->getConstraintSolver();
	btBroadphaseInterface* broadphaseInterface = this->dynamicsWorld->getBroadphase();
	btDispatcher* collisionDispatcher = this->dynamicsWorld->getDispatcher();

	delete this->dynamicsWorld;

	delete constraintSolver;
	delete broadphaseInterface;
	delete collisionDispatcher;

	delete player;
}

btRigidBody* PhysicsEngine::createGroundRigidBodyFromShape(btCollisionShape* groundShape)
{
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -10, 0));

	{
		//The ground is not dynamic, we set its mass to 0
		btScalar mass(0);

		//No need for calculating the inertia on a static object
		btVector3 localInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, groundMotionState, groundShape, localInertia);

		return new btRigidBody(rbInfo);
	}
}