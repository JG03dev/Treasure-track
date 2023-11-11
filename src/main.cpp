#include <vector>

#include "btBulletDynamicsCommon.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <AL/al.h>
#include <AL/alc.h>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "PhysicsEngine.h"

#include "SoundDevice.h"
#include <iostream>
#include "MusicBuffer.h"
#include "MainLoop/MainLoop.h"  // for looping
#include <Windows.h>  // for keyboard press



const float toRadians = 3.14159265f / 180.0f;

Renderer* Renderer::renderer = NULL;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "../../../Shaders/shader.vert";
// Fragment shader
static const char* fShader = "../../../Shaders/shader.frag";

Shader* CreateShader()
{
    auto* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    return shader1;
}

/*
* This should return the Imported, the Device and the context from 
* the libraries, but for now is to test that it compiles and links 
* them correctly.
*/

int initialize_libraries()
{
    // Initialize Assimp
    Assimp::Importer importer;

    // Initialize OpenAL
    ALCdevice* device = alcOpenDevice(NULL);
    if (!device) {
        std::cerr << "Failed to initialize OpenAL device" << std::endl;
        return -1;
    }

    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context) {
        std::cerr << "Failed to initialize OpenAL context" << std::endl;
        return -1;
    }

    alcMakeContextCurrent(context);

    // Since this is just for testing we destroy everything after initialization

    importer.FreeScene();
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    return 0;
}

int main(int argc, char* argv[])
{
    Renderer* renderer = Renderer::getInstance();

    renderer->startWindow();// Todo el c�digo de graficos debe ir despues de esta inicializacion

    PhysicsEngine pE;

	//CreateObjects();
    renderer->addShader(CreateShader());

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);
    camera.setTarget(pE.player);

    renderer->addCamera(&camera);

    // Test other libraries
    if (initialize_libraries() < 0)
        return -1;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

    SoundDevice::init();
    static MusicBuffer myMusic("../../../music/coconut.wav");
	// Loop until window closed
	while (!renderer->mainWindow->getShouldClose())
	{
        myMusic.UpdateBufferStream();
        GLfloat now = (GLfloat) glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        static float musiccontrolcooldown = 1;
        musiccontrolcooldown += deltaTime;
        if (musiccontrolcooldown > 0 && GetKeyState('Q') & 0x8000)
        {
            if (myMusic.isPlaying()) // toggle play/pause
            {
                myMusic.Pause();
            }
            else
            {
                myMusic.Play();
            }
            _sleep(100);
            musiccontrolcooldown = 0;
        }        

        pE.stepSimulation(deltaTime);

		// Get + Handle user input events
		glfwPollEvents();

        pE.player->keyboardCallback(renderer->getKeys(), deltaTime, 1);

        //camera.keyControl(mainWindow.getKeys(), deltaTime);
        renderer->updateCameraPos();
        renderer->updateCameraRotation();

		// Clear window
        renderer->clearWindow();

        renderer->render(pE.groundRB, pE.player->vehicle->getChassisWorldTransform());
	}

	return 0;
}