#ifndef CAMERA_H
#define CAMERA_H

// --OUR INCLUDES--

#pragma region ENCABEZADOS

#include "../Encabezados/stdafx.h"

#pragma endregion

#pragma region GENERAL

#include "../General/Player.h"

#pragma endregion

// --OTHER INCLUDES--

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement_Type {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:

	// Default camera values
	static float YAW;
	static float PITCH;
	static float SPEED;
	static float HIGH_SPEED;
	static float SENSITIVITY;
	static float FOV;

	// Camera attribute
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 worldUp;
	glm::vec3 right;

	// Eular angles
	float yaw;
	float pitch;

	Player* player;

	// Camera options
	float fov;
	float speed;
	float sensitivity;

	Camera( // Init with vector
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH);

	Camera( // Init with scalar values
		float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch);

	// Get view matrix based on Eular angles
	glm::mat4 getViewMatrix();

	glm::vec3 getCameraPos() { return position; }
	float getPitch() { return pitch; }
	float getYaw() { return yaw; }

	// Process keyboard events
	void processKeyboard(Camera_Movement_Type move_type, float deltaTime);

	// Process mouse movement events
	void processMouse(float offsetX, float offsetY, GLboolean constrainPitch = true);

	// Process mouse scroller events
	void processScroll(float offsetY);

	// Adjust moving speed
	void processAccerlate(bool accer = false);

	void followPlayer();

	void setTarget(Player* p) { player = p; }

	void changeCamera();

	// Update vectors of the camera
	void update();
private:
	bool m_firstPerson;
	bool m_isCameraMoving;

};

#endif