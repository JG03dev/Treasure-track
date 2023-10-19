#pragma once
#ifndef GROUND_MESH_H
#define GROUND_MESH_H

#include <GL/glew.h>

//***************************THE FAMOUS G R O U N D********************************************//

const int GROUND_NUM_TRIANGLES = 902;
const int GROUND_NUM_VERTICES = 453;
const int GROUND_NUM_INDICES = GROUND_NUM_TRIANGLES * 3;

static GLfloat gVerticesGround[GROUND_NUM_VERTICES * 3] = {
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

static unsigned int gIndicesBunny[GROUND_NUM_INDICES] = {
        1, 2, 6, 6, 5, 1, // up
        0, 3, 7, 7, 4, 0, // down
        0, 1, 2, 2, 3, 0, // back
        4, 5, 6, 6, 7, 4, // front
        2, 3, 7, 7, 6, 2, // left
        1, 5, 4, 4, 0, 1, // right
};


#endif //GROUND_MESH_H