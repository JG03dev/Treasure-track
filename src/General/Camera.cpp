#include "Camera.h"

float Camera::YAW = -90.0f;
float Camera::PITCH = 0.0f;
float Camera::SPEED = 20.0f;
float Camera::HIGH_SPEED = 20.0f;
float Camera::SENSITIVITY = 0.1f;
float Camera::FOV = 45.0f;

Camera::Camera( // Init with vector
	glm::vec3 position,
	glm::vec3 up,
	float yaw,
	float pitch) :
	position(position), yaw(yaw), pitch(pitch) {
	//this->position = position;
	//this->yaw      = yaw;
	//this->pitch    = pitch;
	worldUp = up;
	front = glm::vec3(0.0f, 0.0f, -1.0f),
		speed = SPEED;
	sensitivity = SENSITIVITY;
	fov = FOV;
	m_firstPerson = false;
	m_isCameraMoving = false;
	update();
}

Camera::Camera( // Init with scalar values
	float posX, float posY, float posZ,
	float upX, float upY, float upZ,
	float yaw, float pitch) :
	position(posX, posY, posZ), yaw(yaw), pitch(pitch) {
	//this->position = glm::vec3(posX, posY, posZ);
	//this->yaw      = yaw;
	//this->pitch    = pitch;
	worldUp = glm::vec3(upX, upY, upZ);
	front = glm::vec3(0.0f, 0.0f, -1.0f),
		speed = SPEED;
	sensitivity = SENSITIVITY;
	fov = FOV;
	m_firstPerson = false;
	m_isCameraMoving = false;
	update();
}

// Get view matrix based on Eular angles
glm::mat4 Camera::getViewMatrix() {
	// lookAt(position, target, up direction)
	return glm::lookAt(position, position + front, up);
}

// Process keyboard events
void Camera::processKeyboard(Camera_Movement_Type move_type, float deltaTime) {
	float velocity = speed * deltaTime;
	if (move_type == FORWARD)  position += front * velocity;
	if (move_type == BACKWARD) position -= front * velocity;
	if (move_type == LEFT)     position -= right * velocity;
	if (move_type == RIGHT)    position += right * velocity;
	if (move_type == UP)       position += worldUp * velocity;
	if (move_type == DOWN)     position -= worldUp * velocity;
}

// Process mouse movement events
void Camera::processMouse(float offsetX, float offsetY, GLboolean constrainPitch) {
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	yaw += offsetX;
	pitch += offsetY;

	if (constrainPitch) {
		if (pitch > 89.0f)  pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}

	update();
}

// Process mouse scroller events
void Camera::processScroll(float offsetY) {
	if (fov >= 1.0f && fov <= 150.0f) fov -= offsetY;
	if (fov < 1.0f) fov = 1.0f;
	if (fov > 150.0f) fov = 150.0f;
}

// Adjust moving speed
void Camera::processAccerlate(bool accer) {
	if (accer) speed = HIGH_SPEED;
	else speed = SPEED;
}

void Camera::changeCamera()
{
	m_firstPerson = !m_firstPerson;
}

void Camera::followPlayer()
{
	glm::vec3 forward = this->player->getCarForward();

	if (m_firstPerson) {
		/* PRIMERA PERSONA */
		glm::vec3 rightCar = glm::normalize(glm::cross(forward, worldUp));
		glm::vec3 pos = this->player->getCarPos() - rightCar * 0.2f;
		position = glm::vec3(float(pos.x), float(pos.y + 0.7), float(pos.z));
		float yaw2, pitch2, roll2;
		player->getCarRotation().getEulerZYX(yaw2, pitch2, roll2);
		float actualYaw = glm::radians(yaw);
		if (forward.x >= 0) {
			actualYaw -= pitch2;
		}
		else {
			actualYaw -= (pitch2 + (glm::pi<float>() / 2 - pitch2) * 2);
		}
		glm::vec3 tmpFront;
		tmpFront.x = cos(glm::radians(pitch)) * cos(actualYaw);
		tmpFront.y = sin(glm::radians(pitch));
		tmpFront.z = cos(glm::radians(pitch)) * sin(actualYaw);
		//std::cout << "Player forward: " << forward.x << ", " << forward.y << ", " << forward.z << std::endl;
		front = glm::normalize(tmpFront);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
	else {
		/* TERCERA PERSONA */
		glm::vec3 pos = this->player->getCarPos() - forward * 4.f;
		front = glm::vec3(forward.x, forward.y - 0.2, forward.z);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
		position = glm::vec3(float(pos.x), float(pos.y + 1.5), float(pos.z));
	}
}

// Update front vector of the camera
void Camera::update() {
	if (m_firstPerson)
	{
		glm::vec3 tmpFront;
		tmpFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		tmpFront.y = sin(glm::radians(pitch));
		tmpFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		front = glm::normalize(tmpFront);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
}