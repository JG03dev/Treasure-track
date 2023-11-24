#include "Object.h"

// Constructors

Object::Object(string const& modelPath, btDiscreteDynamicsWorld* dynamicsWorld) {
	model = new Model(modelPath);

	this->CreateRigidBody();

	dynamicsWorld->addRigidBody(this->rb);
}

// Public Methods

void Object::Draw(Shader& shader) {
	model->Draw(shader);
}

// Private Methods

void Object::CreateRigidBody() {
	btCompoundShape* objectShape = new btCompoundShape();

	btTransform objectTransform;
	objectTransform.setIdentity();
	objectTransform.setOrigin(btVector3(0, 0, 0));

	for (int i = 0, numMeshes = model->meshes.size(); i < numMeshes; i++) {
		btTriangleMesh* triangleMesh = new btTriangleMesh();

		Mesh* mesh = &this->model->meshes.front();
		for (int i = 0; i < mesh->indices.size(); i += 3) {
			btVector3 v1(mesh->vertices[i].Position.x, mesh->vertices[i].Position.y, mesh->vertices[i].Position.z);
			btVector3 v2(mesh->vertices[i + 1].Position.x, mesh->vertices[i + 1].Position.y, mesh->vertices[i + 1].Position.z);
			btVector3 v3(mesh->vertices[i + 2].Position.x, mesh->vertices[i + 2].Position.y, mesh->vertices[i + 2].Position.z);

			triangleMesh->addTriangle(v1, v2, v3);
		}

		btCollisionShape* partialShape = new btBvhTriangleMeshShape(triangleMesh, true);
		objectShape->addChildShape(objectTransform, partialShape);
	}

	btScalar mass(0);

	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* objectMotionState = new btDefaultMotionState(objectTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, objectMotionState, objectShape, localInertia);

	this->rb = new btRigidBody(rbInfo);
}