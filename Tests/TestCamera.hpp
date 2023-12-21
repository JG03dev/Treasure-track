#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include "../src/General/Camera.h"
#include "Utility.hpp"

int testCamera() {
    int fails = 0;
    // Test constructor with vector
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    float yaw = 45.0f;
    float pitch = -30.0f;

    Camera cameraVector(position, up, yaw, pitch);
    glm::mat4 res(-0.707106769f, -0.707106769f, -0.0f, 0.0f,
        0.707106769f, -0.707106769f, -0.0f, 0.0f,
        0.0f, -0.0f, 1.0f, 0.0f,
        -0.707106769f, 2.12132025f, -3.0f, 1.0f);
    // Check if values are initialized correctly
    fails += Assert_bool(cameraVector.getViewMatrix() == res, "Constructor with vector");

    // Test constructor with scalar values
    float posX = 1.0f, posY = 2.0f, posZ = 3.0f;
    float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

    Camera cameraScalar(posX, posY, posZ, upX, upY, upZ, yaw, pitch);

    // The default view matrix should be an identity matrix for the default position and orientation
    fails += Assert_bool(cameraScalar.getViewMatrix() == res, "Constructor with scalar values");
    

    return fails;
}
#endif
