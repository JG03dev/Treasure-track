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
    m_renderer = new Renderer("../../../Assets/Objects.json", m_SCR_WIDTH, m_SCR_HEIGHT);
    //Iniciamos objetos
    
    Model* p = m_renderer->getModel("Player").first;

    //Load objects
    //TODO: find some way to init ChassisWorldTransform based on object TG (stored at Obj.second.second)
    //TODO: Aquesta part pot ser bastant lenta, es pot mirar de renderitzar una pantalla de carga abans que aixo.
    for (auto& Obj : m_renderer->getModelList())
    {
        Obj.second.first->Load();
        glm::mat4 model(1.0f);
        if (Obj.first == "Player")
        {
            m_Player = new Player(p, m_dynamicsWorld);
            // This could be at the constructor
            m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
            m_renderer->setModelMatrix(Obj.first, model);
        }
        else 
        {
            m_Objects.push_back(new Object(Obj.second.first, m_dynamicsWorld));
            m_Objects.back()->rb->getWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
            m_renderer->setModelMatrix(Obj.first, model);
        }        
    }
    if (!m_Player)
    {
        //Handle error when no player is parsed
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
    MySoundEffects sound;

    while (!glfwWindowShouldClose(m_Window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sound.PlaySound(deltaTime);

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

    m_renderer->RenderEverything(*m_Camera, projection);
}
#pragma endregion
