#pragma once

#include "Mesh.h"
#include "btBulletDynamicsCommon.h"

class PhysicsObject {
public:
	
protected:
    unsigned int numOfVertices;
    unsigned int numOfIndices;

    float scale[3];
    float position[3];

    Mesh* mesh;
    btRigidBody* rb;

    btCollisionShape* collisionShape;
    btDefaultMotionState* motionState;
private:

};