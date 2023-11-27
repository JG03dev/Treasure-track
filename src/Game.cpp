#include "Game.h"

#pragma region PUBLIC_METHODS

void Game::StartGame() {

    InitializePhysics();
    InitializeGraphics();

    Run();
}
Game::~Game()
{
    delete m_Player, m_Camera, m_skybox, m_dynamicsWorld;
    for (int i = 0; i < m_Objects.size(); i++)
        delete m_Objects[i];

    delete m_renderer;
    
}
#pragma endregion

#pragma region PRIVATE_METHODS_INITIALIZERS

void Game::InitializePhysics()
{
    btVector3 gravity(0, -9.8, 0);

    //Collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

    //Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

    //The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    //Creates the dynamics world, wich will be responsable for managing our physics objects and constraints
    this->m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    this->m_dynamicsWorld->setGravity(gravity);
}

void Game::InitializeGraphics()
{
    // Init renderer
    std::vector<std::string> faces = {
        "../../../Assets/skybox/right.jpg",			// right
        "../../../Assets/skybox/left.jpg",			// left
        "../../../Assets/skybox/top.jpg",			// top
        "../../../Assets/skybox/bottom.jpg",		// bottom
        "../../../Assets/skybox/front.jpg",			// front
        "../../../Assets/skybox/back.jpg"			// back
    };
    m_skybox = new Skybox(faces);



	m_renderer = new Renderer(  "../../../Shaders/shader.vert", "../../../Shaders/shader.frag", NULL,
                            "../../../Shaders/skybox.vert", "../../../Shaders/skybox.frag", NULL,
                            "../../../Shaders/directional_shadow_map.vert", "../../../Shaders/directional_shadow_map.frag", NULL,
		                    "../../../Shaders/omni_shadow_map.vert", "../../../Shaders/omni_shadow_map.frag", "../../../Shaders/omni_shadow_map.geom",
                            m_skybox, m_SCR_WIDTH, m_SCR_HEIGHT);

    // Inicializacion de luces
    DirectionalLight* mainLight = new DirectionalLight(2048, 2048, /*Shadow dimensions*/
        1.0f, 1.0f, 1.0f, /*RGB colour*/
        0.9f, 0.5f,	/*Intensity (ambient, diffuse)*/
        -10.0f, -12.0f, 18.5f /*Direction of the light*/
    );
    m_renderer->AddLight(mainLight);
    
    //Iniciamos objetos
    m_Player = new Player("../../../Assets/cotxe/cotxe.obj", "cotxe", m_dynamicsWorld, 4.0f, 256);

    m_Objects.push_back(new Object("../../../Assets/town/town.obj", "town", m_dynamicsWorld, 4.0f, 256));

	{
		glm::mat4 model(1.0f);
		m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
		m_renderer->AddModel(m_Player->model->GetName(), m_Player->model, model);
	}

	for (Object* o : m_Objects) {
		glm::mat4 model(1.0f);
		o->rb->getWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
        m_renderer->AddModel(m_Objects[0]->model->GetName(), m_Objects[0]->model, model);
	}

    // Inicializar camara
    //m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    m_Camera->setTarget(m_Player);
}

int Game::InitializeWindow()
{
    // glfw window creation
    // --------------------
    m_Window = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "VGI-ABP", NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_Window);


    return 0;
}

#pragma endregion

#pragma region PRIVATE_METHODS_FUNCTIONALITY

void Game::Run()
{
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(m_Window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input: Manejar entrada de usuario
        // -----
        glfwPollEvents();

        //Actualizar Informacion (Mover coordenadas)
        Actualizar(deltaTime);

        //Renderizar
        Render();

        glfwSwapBuffers(m_Window);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::ProcessInput(GLFWwindow* window, int key, int action)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    m_Player->InputMethod(key, action);

    // //W
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     m_Player->InputMethod(GLFW_KEY_W, true);
    // 
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
    //     m_Player->InputMethod(GLFW_KEY_W, false);
    // 
    // //S
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     m_Player->InputMethod(GLFW_KEY_S, true);
    // 
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
    //     m_Player->InputMethod(GLFW_KEY_S, false);
    // 
    // //A
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     m_Player->InputMethod(GLFW_KEY_A, true);
    // 
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
    //     m_Player->InputMethod(GLFW_KEY_A, false);
    // 
    // //D
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     m_Player->InputMethod(GLFW_KEY_D, true);
    // 
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
    //     m_Player->InputMethod(GLFW_KEY_D, false);
    // 
    // //Ctrl
    // if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    //     m_Player->InputMethod(GLFW_KEY_LEFT_CONTROL, true);
    // 
    // if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    //     m_Player->InputMethod(GLFW_KEY_LEFT_CONTROL, false);
}

void Game::Actualizar(float deltaTime)
{
    m_dynamicsWorld->stepSimulation(deltaTime, 2);
}

void Game::Render()
{
    {
		glm::mat4 model(1.0f);
		m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
        //std::cout << "Player position:" << m_Player->vehicle->getChassisWorldTransform().getOrigin().getY() << std::endl;
		m_renderer->setModelMatrix(m_Player->model->GetName(), model);
    }

    // Update camera
    m_Camera->followPlayer();

    glm::mat4 projection = glm::perspective(glm::radians(m_Camera->FOV), (float)m_SCR_WIDTH / (float)m_SCR_HEIGHT, c_near, c_far);

    m_renderer->RenderEverything(m_Camera->getViewMatrix(), projection, *m_Camera);
}
#pragma endregion
