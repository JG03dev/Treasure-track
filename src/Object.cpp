#include "Object.h"

// Constructors

Object::Object(string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld) {
	model = new Model(modelPath);

	this->CreateRigidBody();

	dynamicsWorld->addRigidBody(this->rb);
}

// Private Methods

void Object::CreateRigidBody() {
	btTriangleMesh* triangleMesh = new btTriangleMesh();

	Mesh* mesh = &this->model->meshes.front();
	for (int i = 0; i < mesh->indices.size(); i += 3) {
		btVector3 v1(mesh->vertices[i].Position.x, mesh->vertices[i].Position.y, mesh->vertices[i].Position.z);
		btVector3 v2(mesh->vertices[i + 1].Position.x, mesh->vertices[i + 1].Position.y, mesh->vertices[i + 1].Position.z);
		btVector3 v3(mesh->vertices[i + 2].Position.x, mesh->vertices[i + 2].Position.y, mesh->vertices[i + 2].Position.z);

		triangleMesh->addTriangle(v1, v2, v3);
	}

	btCollisionShape* objectShape = new btBvhTriangleMeshShape(triangleMesh, true);

	btTransform objectTransform;
	objectTransform.setIdentity();
	objectTransform.setOrigin(btVector3(0, 0, 0));

	btScalar mass(0);

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* objectMotionState = new btDefaultMotionState(objectTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, objectMotionState, objectShape, localInertia);

	this->rb = new btRigidBody(rbInfo);
}