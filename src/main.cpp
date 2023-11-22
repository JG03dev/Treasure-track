#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "stdafx.h"

/** Camera Wrapper */
#include "Camera.h"

/** Model Wrapper */
#include "Model.h"
#include "Primitives.h"
#include "Skybox.h"
#include "Shadow.h"

// Global Variables
const char* APP_TITLE = "VGI-ABP";
const int gWindowWidth = 1280;
const int gWindowHeight = 720;
GLFWwindow* gWindow = NULL;

// Camera system
Camera camera(glm::vec3(0.0f, 0.0f, 30.0f));
float c_near = 0.1f;
float c_far = 500.0f;

// Shader control
bool enableTorch = true;
bool enableNormal = true;
float adjustGamma = 2.2f;
float adjustParallax = 0.01f;

// Function prototypes
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();
void renderScene(Shader& shader);

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
	Shader objectShader, skyboxShader, shadowShader;
	objectShader.loadShaders("../../../Shaders/object.vert", "../../../Shaders/object.frag");
	skyboxShader.loadShaders("../../../Shaders/skybox.vert", "../../../Shaders/skybox.frag");
	shadowShader.loadShaders("../../../Shaders/shadow.vert", "../../../Shaders/shadow.frag");
	// TODO: setup general lighting shader


	// Model loader
	pObjCity1 = std::make_shared<Model>("../../../Assets/cotxe/cotxe.obj");
	//pObjCar1 = std::make_shared<Model>("");

	// Shadow
	ParallelShadow shadowMap;

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
	glm::vec3 pointLightPos[] = {
		glm::vec3(3.0f,  0.0f,  0.0f),
		glm::vec3(-3.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f, -3.0f),
		glm::vec3(0.0f,  0.0f,  3.0f)
	};
	glm::vec3 directionalLightDirection(-1.0f, 0.0f, 0.0f);

	// Object shader config
	objectShader.use();
	// Light config
	// Directional light
	objectShader.setUniform("uDirectionalLight.direction", directionalLightDirection);
	objectShader.setUniform("uDirectionalLight.ambient", 0.1f, 0.1f, 0.1f);
	objectShader.setUniform("uDirectionalLight.diffuse", 0.5f, 0.5f, 0.5f);
	objectShader.setUniform("uDirectionalLight.specular", 0.3f, 0.3f, 0.3f);
	// Spot light
	objectShader.setUniform("uSpotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
	objectShader.setUniform("uSpotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	objectShader.setUniform("uSpotLight.ambient", 0.1f, 0.1f, 0.1f);
	objectShader.setUniform("uSpotLight.diffuse", 0.5f, 0.5f, 0.5f);
	objectShader.setUniform("uSpotLight.specular", 0.7f, 0.7f, 0.7f);
	objectShader.setUniform("uSpotLight.constant", 1.0f);
	objectShader.setUniform("uSpotLight.linear", 0.09f);
	objectShader.setUniform("uSpotLight.quadratic", 0.032f);



	// Camera global
	float aspect = (float)gWindowWidth / (float)gWindowHeight;



	// Rendering loop
	while (!glfwWindowShouldClose(gWindow)) {

		// Display FPS on title
		showFPS(gWindow);

		// Key input
		processInput(gWindow);

		// Create transformations
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), aspect, c_near, c_far);

		/** Shadow */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// get light transformation
		glm::mat4 lightProjection, lightView;
		float near_plane = 1.0f, far_plane = 100.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		// render scene from light's point of view
		shadowShader.use();
		shadowShader.setUniform("uView", lightView);
		shadowShader.setUniform("uProjection", lightProjection);
		// get shadow map
		glViewport(0, 0, shadowMap.width, shadowMap.height);
		shadowMap.Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		renderScene(shadowShader);
		glCullFace(GL_BACK);
		shadowMap.Unbind();



		/** General scene */
#ifdef __APPLE__
		glViewport(0, 0, 2 * gWindowWidth, 2 * gWindowHeight);
#else
		glViewport(0, 0, gWindowWidth, gWindowHeight);
#endif
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Skybox
		glm::mat4 staticView = glm::mat4(glm::mat3(view)); // remove translation composition
		skybox.Draw(skyboxShader, staticView, projection);
		// Object shader
		objectShader.use();
		objectShader.setUniform("uView", view);
		objectShader.setUniform("uProjection", projection);
		objectShader.setUniform("uCameraPos", camera.position);
		// Spot light
		objectShader.setUniform("uEnableTorch", enableTorch);
		objectShader.setUniform("uSpotLight.position", camera.position);
		objectShader.setUniform("uSpotLight.direction", camera.front);
		// Shadow map
		objectShader.setUniform("uLightSpaceMatrix", lightProjection * lightView);
		objectShader.setUniform("uShadowMap", (int)shadowMap.active_texture_unit);
		glActiveTexture(GL_TEXTURE0 + shadowMap.active_texture_unit);
		glBindTexture(GL_TEXTURE_2D, shadowMap.TID());
		// Draw scene
		renderScene(objectShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwPollEvents();
		glfwSwapBuffers(gWindow);
	}

	glfwTerminate();

	return 0;
}

void renderScene(Shader& shader) {

	// Set geological configurations
	float currentTime = (float)glfwGetTime();

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	shader.use();
	shader.setUniform("uModel", model);
	shader.setUniform("uTime", currentTime);
	shader.setUniform("uGamma", adjustGamma);
	shader.setUniform("uHeightScale", adjustParallax);
	if (enableNormal)
		shader.setUniform("uEnableNormal", true);
	shader.setUniform("uEnableEmission", true);
	pObjCity1.get()->Draw(shader);
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
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