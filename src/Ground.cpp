#include "Ground.h"

Ground::Ground(float* position, float* scale) {
    int i;
    for (i = 0; i < 3; i++) { this->scale[i] = scale[i]; }
    for (i = 0; i < 3; i++) { this->position[i] = position[i]; }

    MeshInit();

    PhysicsInit();
}

void Ground::MeshInit() {
    
    unsigned int indices[] = {
        1, 2, 6, 6, 5, 1, // up
        0, 3, 7, 7, 4, 0, // down
        0, 1, 2, 2, 3, 0, // back
        4, 5, 6, 6, 7, 4, // front
        2, 3, 7, 7, 6, 2, // left
        1, 5, 4, 4, 0, 1, // right
    };

    GLfloat vertices[] = {
        //  X, Y, Z
            1.f, 0.f, 0.f,
            1.f, 1.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 0.f, 0.f,
            1.f, 0.f, 1.f,
            1.f, 1.f, 1.f,
            0.f, 1.f, 1.f,
            0.f, 0.f, 1.f,
    };

    numOfIndices = 36;
    numOfVertices = 24;

    mesh = new Mesh();
    mesh->CreateMesh(vertices, indices, numOfVertices, numOfIndices);
}

void Ground::PhysicsInit() {
    collisionShape = new btBoxShape(btVector3(btScalar(scale[0]), btScalar(scale[1]), btScalar(scale[2])));

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(btScalar(position[0]), btScalar(position[1]), btScalar(position[2])));

    btScalar mass(0.);

    btVector3 localInertia(0, 0, 0);

    motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
    rb = new btRigidBody(rbInfo);
}