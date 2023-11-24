//main.cpp
#include "include/stdafx.h"

/** Camera Wrapper */
#include "General/Camera.h"

/** Model Wrapper */
#include "Modelo/Model.h"
#include "General/Skybox.h"
#include "Luces/DirectionalLight.h"
#include "Luces/PointLight.h"
#include "Luces/SpotLight.h"
#include "Sombras/ShadowMap.h"
#include "Sombras/OmniShadowMap.h"

#include "globals.h"
#include "Renderer.h"

// Function prototypes
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();
void renderScene(Shader& shader);

//Merge-Note: Models (except for important ones) shold be mapped outside the code to later load them in loop
//-----------------------------------------------------------------------------
// Models
//-----------------------------------------------------------------------------
std::shared_ptr<Model> pObjCity1, pObjCar1;


//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main() {

	if (!initOpenGL()) {
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	// Shader loader
	Shader objectShader, skyboxShader, directionalShadowShader, omniShadowShader;
	objectShader.loadShaders("../../../Shaders/shader.vert", "../../../Shaders/shader.frag");
	skyboxShader.loadShaders("../../../Shaders/skybox.VERT", "../../../Shaders/skybox.FRAG");
	directionalShadowShader.loadShaders("../../../Shaders/directional_shadow_map.vert", "../../../Shaders/directional_shadow_map.frag");
	omniShadowShader.loadShaders("../../../Shaders/omni_shadow_map.vert", "../../../Shaders/omni_shadow_map.frag", "../../../Shaders/omni_shadow_map.geom");


	// Model loader
	pObjCity1 = std::make_shared<Model>("../../../Assets/town/town.obj");
	pObjCar1 = std::make_shared<Model>("../../../Assets/cotxe/cotxe.obj");

	/** Skybox Mapping Order
		  _______
		  | Top |
		  |  3  |
	______|_____|____________
	|Left |Front|Right| Back|
	|  2  |  5  |  1  |  6  |
	|_____|_____|_____|_____|
		  |Botom|
		  |  4  |
		  |_____|
	*/

	Skybox skybox;
	std::vector<std::string> faces = {
		"../../../Assets/skybox/right.jpg",			// right
		"../../../Assets/skybox/left.jpg",			// left
		"../../../Assets/skybox/top.jpg",			// top
		"../../../Assets/skybox/bottom.jpg",		// bottom
		"../../../Assets/skybox/front.jpg",			// front
		"../../../Assets/skybox/back.jpg"			// back
	};
	skybox.LoadTexture(faces);



	// Light global
	DirectionalLight mainLight(2048, 2048, /*Shadow dimensions*/
		1.0f, 0.53f, 0.3f, /*RGB colour*/
		0.1f, 0.5f,	/*Intensity (ambient, diffuse)*/
		-10.0f, -12.0f, 18.5f /*Direction of the light*/
	);

	SpotLight Light1(1024, 1024,/*Shadow dimensions*/
		0.01f, 100.0f,  /*Light reach (near far)*/
		1.0f, 1.0f, 1.0f, /*RGB colour*/
		0.0f, 2.0f, /*Intensity (ambient, diffuse)*/
		0.0f, 0.0f, 0.0f, /*Position of the light (x, y, z)*/
		0.0f, -1.0f, 0.0f, /*Direction of the light*/
		1.0f, 0.0f, 0.0f, /*Light parameters (constant, linear, exponential)*/
		20.0f /*edge*/
	);


	// Object shader config
	objectShader.use();
	// Light config
	// Directional light
	mainLight.UseLight(objectShader);
	
	// Spot light
	Light1.UseLight(objectShader, 0);

	// Camera global
	float aspect = (float)gWindowWidth / (float)gWindowHeight;

	// Initialize renderer based on what we loaded

	// Rendering loop
	while (!glfwWindowShouldClose(gWindow)) {

		// Display FPS on title
		showFPS(gWindow);

		// Key input
		processInput(gWindow);

		// Create transformations
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), aspect, c_near, c_far);

		// Set geological configurations
		float currentTime = (float)glfwGetTime();
		// Render Everything Up
				
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwPollEvents();
		glfwSwapBuffers(gWindow);
	}

	glfwTerminate();

	return 0;
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
//Merge-Note: This should go to a Game class constructor
bool initOpenGL() {

	// Intialize GLFW 
	// GLFW is configured.  Must be called before calling any GLFW functions
	if (!glfwInit()) {
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// forward compatible with newer versions of OpenGL as they become available
	// but not backward compatible (it will not run on devices that do not support OpenGL 3.3
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	// Set the required callback functions
	//glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetCursorPosCallback(gWindow, mouseCallback);
	glfwSetScrollCallback(gWindow, scrollCallback);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);

	// Hide the cursor and capture it
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLenum err = glewInit();

	// Depth test
	glEnable(GL_DEPTH_TEST);

	return true;
}

//Merge-Note: All this code below should be to a I/O or Window handler class

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processAccerlate(true);
	else
		camera.processAccerlate(false);

	static float deltaTime = 0.0f;
	static float lastFrame = 0.0f;
	// Per-frame time
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.processKeyboard(DOWN, deltaTime);

	static bool gWireframe = false;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		gWireframe = !gWireframe;
		if (gWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		enableTorch = !enableTorch;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		enableNormal = !enableNormal;

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		adjustGamma = adjustGamma >= 4.0f ? 4.0f : adjustGamma + 0.01f;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		adjustGamma = adjustGamma <= 1.0f ? 1.0f : adjustGamma - 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
		adjustParallax = adjustParallax >= 1.0f ? 1.0f : adjustParallax + 0.0005f;
	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
		adjustParallax = adjustParallax <= 0.0f ? 0.0f : adjustParallax - 0.0005f;
}

//-----------------------------------------------------------------------------
// Is called whenever mouse movement is detected via GLFW
//-----------------------------------------------------------------------------
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

	static bool  firstMouse = true;
	static float lastX = gWindowWidth / 2;
	static float lastY = gWindowHeight / 2;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coord range from buttom to top
	lastX = xpos;
	lastY = ypos;

	camera.processMouse(xoffset, yoffset);
}

//-----------------------------------------------------------------------------
// Is called whenever scroller is detected via GLFW
//-----------------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processScroll(yoffset);
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}