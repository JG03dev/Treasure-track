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
#include "Ground.h"

const float toRadians = 3.14159265f / 180.0f;

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

void CreateShader()
{
    auto* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
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
    Window mainWindow = Window(800, 600);
    mainWindow.Initialise(); // Todo el código de graficos debe ir despues de esta inicializacion

    //----------------Physics-----------------
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

    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    float position[3] = { 0.f, -5.f, 0.f };
    float scale[3] = { 50.f, 0.5f, 50.f };
    Ground ground(position, scale);

    collisionShapes.push_back(ground.GetCollisionShape());
    meshList.push_back(ground.GetMesh());

    dynamicsWorld->addRigidBody(ground.GetRb());

	//CreateObjects();
    CreateShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

    // collisionShapes.push_back(camera.GetCollisionShape());
    // dynamicsWorld->addRigidBody(camera.GetRb());

    {
        //create a dynamic rigidbody

        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(1.f), btScalar(1.f), btScalar(1.f)));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);
        btVector3 localInertia(0, 0, 0);
        colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(10, 50, 10));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);

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

        Mesh* cubeMesh = new Mesh();
        cubeMesh->CreateMesh(vertices, indices, 24, 36);
        meshList.push_back(cubeMesh);
    }

    // Test other libraries
    if (initialize_libraries() < 0)
        return -1;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
        dynamicsWorld->stepSimulation(1.f / 60.f);

        GLfloat now = (GLfloat) glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

        glm::mat4 model2;
        btRigidBody* body = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[1]);
        btTransform t;
        body->getMotionState()->getWorldTransform(t);

        //camera.keyControl(mainWindow.getKeys(), deltaTime);
        body->applyCentralImpulse(keyControl(mainWindow.getKeys(), deltaTime, camera.getFront(), camera.getRight()));
        camera.setPosition(glm::vec3(t.getOrigin()[0], t.getOrigin()[1], t.getOrigin()[2]));
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);
        float* pos = ground.GetPosition();
        model = glm::translate(model, glm::vec3(pos[0], pos[1], pos[2]));
        float* size = ground.GetScale();
		model = glm::scale(model, glm::vec3(size[0], size[1], size[2]));
		glUniformMatrix4fv((int) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv((int) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        meshList[0]->RenderMesh();

        // glm::mat4 model2;
        // btRigidBody* body = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[1]);
        // btTransform t;
        // body->getMotionState()->getWorldTransform(t);
        // t.getOpenGLMatrix(glm::value_ptr(model2));
        // model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv((int)uniformModel, 1, GL_FALSE, glm::value_ptr(model2));

        meshList[1]->RenderMesh();

        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        //glUniformMatrix4fv((int) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        //
        //meshList[1]->RenderMesh();


        glUseProgram(0);

		mainWindow.swapBuffers();

	}

	return 0;
}