//
// Created by jg03dev on 10/10/23.
//
#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include "PlayerVehicle.h"

class Camera
{
public:
    Camera() {} // Maybe I'll put some default values
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed);

    void setPosition(glm::vec3 pos) { position = pos; }
    //void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    glm::vec3 getFront() { return front; }
    glm::vec3 getRight() { return right; }

    glm::mat4 calculateViewMatrix();

    void followPlayer();



    ~Camera() {}

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;

    PlayerVehicle* player;

    void update();
};

