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

void CreateObjects()
{
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

    //unsigned int indices[] = {
    //  0, 3, 1,
    //  1, 3, 2,
    //  2, 3, 0,
    //  0, 1, 2
    //};

	//GLfloat vertices[] = {
	//	-1.0f, -1.0f, 0.0f,
    //    0.0f, -1.0f, 1.0f,
	//	1.0f, -1.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f
	//};

    Mesh *obj1 = new Mesh();
    // TODO: clear magic numbers
    obj1->CreateMesh(vertices, indices, 24, 36);
    meshList.push_back(obj1);

    //Mesh *obj2 = new Mesh();
    //obj2->CreateMesh(vertices, indices, 12, 12);
    //meshList.push_back(obj2);


}

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

void  initialize_physics() {
    int i;
    ///-----initialization_start-----

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    
    dynamicsWorld->setGravity(btVector3(0.f, -9.8f, 0.f));

    ///-----initialization_end-----
}

btVector3 keyControl(bool* keys, GLfloat deltaTime, glm::vec3 front, glm::vec3 right)
{
    btVector3 force(0.f, 0.f, 0.f);
    GLfloat velocity = 5.f * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        return btVector3(front.x, front.y, front.z * velocity);
    }
    if (keys[GLFW_KEY_S])
    {
        return btVector3(front.x, front.y, -front.z * velocity);
    }
    if (keys[GLFW_KEY_A])
    {
        return btVector3(right.x, right.y, -right.z * velocity);
    }
    if (keys[GLFW_KEY_D])
    {
        return btVector3(right.x, right.y, -right.z * velocity);
    }

    return force;
}

int main(int argc, char* argv[])
{
    Renderer* renderer = Renderer::getInstance();

    renderer->startWindow();// Todo el código de graficos debe ir despues de esta inicializacion

    PhysicsEngine pE;

	//CreateObjects();
    renderer->addShader(CreateShader());

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

    renderer->addCamera(&camera);

    // Test other libraries
    if (initialize_libraries() < 0)
        return -1;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	// Loop until window closed
	while (!renderer->mainWindow->getShouldClose())
	{
        GLfloat now = (GLfloat) glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        pE.stepSimulation(deltaTime);

		// Get + Handle user input events
		glfwPollEvents();

        //camera.keyControl(mainWindow.getKeys(), deltaTime);
        renderer->updateCameraPos();
        renderer->updateCameraRotation();

		// Clear window
        renderer->clearWindow();

        renderer->render(pE.groundRB);
	}

	return 0;
}