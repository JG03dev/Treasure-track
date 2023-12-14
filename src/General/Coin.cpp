#include "Coin.h"


void Coin::CreateGhostObject(Model& hitbox)
{
	btConvexHullShape convexShape;

	//TODO: Calcular transform de cada mesh en funcion de su posicion inicial con vertices
	Mesh* mesh = hitbox.meshList[0];
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

	this->ghostObject = new btPairCachingGhostObject();
	
	// Configurar el objeto fantasma
	ghostObject->setCollisionShape(box);
	ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void Coin::setOpenGLMatrixToPhysics(const glm::mat4 openGLMatrix)
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
	ghostObject->setWorldTransform(transform);

	// Activate the rigid body to apply the changes
	ghostObject->activate();
}