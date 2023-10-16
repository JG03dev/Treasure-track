#pragma once

#include <GL/glew.h>

#include "btBulletDynamicsCommon.h"

#include "Mesh.h"

class Ground {
public:
	Ground(float* position, float* scale);

    void MeshInit();
    void PhysicsInit();

    Mesh* GetMesh() { return mesh; }

    btCollisionShape* GetCollisionShape() { return collisionShape; }
    btDefaultMotionState* GetMotionState() { return motionState; }
    btRigidBody* GetRb() { return rb; }
    float* GetPosition() { return position; }
    float* GetScale() { return scale; }

private:
	unsigned int numOfVertices;
	unsigned int numOfIndices;

    float scale[3];
    float position[3];

    Mesh* mesh;
    btRigidBody* rb;

    btCollisionShape* collisionShape;
    btDefaultMotionState* motionState;
    
};
