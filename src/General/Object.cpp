#include "Object.h"

// Constructors

Object::Object(std::string const& modelPath, std::string const& modelName, btDiscreteDynamicsWorld* dynamicsWorld, GLfloat sIntensity, GLfloat shine) {
	model = new Model(sIntensity, shine);
	model->LoadModel(modelPath, modelName);

	this->CreateRigidBody();

	dynamicsWorld->addRigidBody(this->rb);
}

// Public Methods

void Object::Draw(Shader& shader) {
	//model->Draw(shader);
}

// Private Methods

void Object::CreateRigidBody() {
	btCompoundShape* objectShape = new btCompoundShape();

	for (int i = 0, numMeshes = model->meshList.size(); i < numMeshes; i++) {
		btConvexHullShape convexShape;

		//TODO: Calcular transform de cada mesh en funcion de su posicion inicial con vertices
		Mesh* mesh = this->model->meshList[i];
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

		btBoxShape* box = new btBoxShape(halfExtents);

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