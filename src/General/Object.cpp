#include "Object.h"

// Constructors
// TODO: I would delete this constructor since it would lead to memory leaks
Object::Object(std::string const& modelPath, std::string const& modelName, btDiscreteDynamicsWorld* dynamicsWorld, GLfloat sIntensity, GLfloat shine) {
	model = new Model(sIntensity, shine);
	model->LoadModel(modelPath, modelName);

	Model hitbox;
	hitbox.LoadModel(std::string("../../../Assets/") + modelName + std::string("/") + modelName + std::string("Hitbox.obj"), modelName + std::string("Hitbox"));
	this->CreateRigidBody(hitbox);

	dynamicsWorld->addRigidBody(this->rb);
}

Object::Object(Model* m, btDiscreteDynamicsWorld* dynamicsWorld)
{
	model = m; // Model points to the same model shared

	Model hitbox;
	hitbox.LoadModel(std::string("../../../Assets/") + m->GetName() + std::string("/") + m->GetName() + std::string("Hitbox.obj"), m->GetName() + std::string("Hitbox"));
	this->CreateRigidBody(hitbox);

	dynamicsWorld->addRigidBody(this->rb);
}

// Public Methods

void Object::Draw(Shader& shader) {
	//model->Draw(shader);
}

// Private Methods

void Object::CreateRigidBody(Model& hitbox) {
	btCompoundShape* objectShape = new btCompoundShape();

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

		btBoxShape* box = new btBoxShape(halfExtents);
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