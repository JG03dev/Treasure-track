#include "Object.h"

// Constructors
Object::Object(Model* m, btDiscreteDynamicsWorld* dynamicsWorld, glm::mat4 initOpenGLMatrix)
{
	model = m; // Model points to the same model shared

	Model hitbox;
	hitbox.LoadModel(std::string("../../../Assets/") + m->GetName() + std::string("/") + m->GetName() + std::string("Hitbox.obj"), m->GetName() + std::string("Hitbox"));
	this->CreateRigidBody(hitbox);

	setOpenGLMatrixToPhysics(initOpenGLMatrix);


	dynamicsWorld->addRigidBody(this->rb);
}

// Destructors

void Object::DestroyObject(btDiscreteDynamicsWorld* dynamicsWorld) {
	if (rb && rb->getMotionState()) {
		while (rb->getNumConstraintRefs()) {
			btTypedConstraint* constraint = rb->getConstraintRef(0);
			dynamicsWorld->removeConstraint(constraint);
			delete constraint;
		}
		delete rb->getMotionState();
		dynamicsWorld->removeRigidBody(rb);
	}

	for (int i = 0; i < m_CollisionShapes.size(); i++) {
		btCollisionShape* shape = m_CollisionShapes[i];
		delete shape;
	}
	m_CollisionShapes.clear();
}

// Public Methods

void Object::setOpenGLMatrixToPhysics(const glm::mat4 openGLMatrix)
{
	// Create a transformation matrix from the OpenGL matrix
	btTransform transform;

	// Extract the translation, rotation, and scaling components from the OpenGL matrix
	glm::vec3 translation, scale;
	glm::quat rotation;
	glm::vec4 perspective;
	glm::vec3 skew;
	glm::vec4 perspe;

	// Decompose the OpenGL matrix into translation, rotation, and scale
	glm::decompose(openGLMatrix, scale, rotation, translation, skew, perspe);

	// Set the translation part of the transformation
	transform.setOrigin(btVector3(translation.x, translation.y, translation.z));

	// Set the rotation part of the transformation
	transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

	// Apply the transformation to the rigid body
	rb->setWorldTransform(transform);

	// Activate the rigid body to apply the changes
	rb->activate();
}

// Private Methods

void Object::CreateRigidBody(Model& hitbox) {
	btCompoundShape* objectShape = new btCompoundShape();
	m_CollisionShapes.push_back(objectShape);

	for (int i = 0, numMeshes = hitbox.meshList.size(); i < numMeshes; i++) {
		btConvexHullShape convexShape;

		//TODO: Calcular transform de cada mesh en funcion de su posicion inicial con vertices
		Mesh* mesh = hitbox.meshList[i];
		const std::vector<GLfloat>* vertices = mesh->GetVertices();
		for (int j = 0; j < vertices->size(); j += 8) {
            btVector3 v1((*vertices)[j], (*vertices)[j + 1], (*vertices)[j + 2]);

            convexShape.addPoint(v1);
		}
		btVector3 min, max;
		
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0, 0, 0));
        convexShape.getAabb(t, min, max);
		btVector3 halfExtents = (max - min) / 2;

		btCollisionShape* box = new btBoxShape(halfExtents);
		m_CollisionShapes.push_back(box);

		box->setMargin(0.01f);
		btTransform objectTransform;
		objectTransform.setIdentity();
		objectTransform.setOrigin(min + halfExtents);

		objectShape->addChildShape(objectTransform, box);
	}

	btScalar mass(0);

	btVector3 localInertia(0, 0, 0);

	btTransform compoundTransform;
	compoundTransform.setIdentity();
	compoundTransform.setOrigin(btVector3(0, 0, 0));

	btDefaultMotionState* objectMotionState = new btDefaultMotionState(compoundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, objectMotionState, objectShape, localInertia);

	this->rb = new btRigidBody(rbInfo);
}