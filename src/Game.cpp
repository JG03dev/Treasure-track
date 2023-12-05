#include "Game.h"

#pragma region PUBLIC_METHODS



void Game::StartGame() {
    img_loader();
    InitializePhysics();
    InitializeGraphics();
    Run();
}
Game::~Game()
{
    delete m_renderer;
    delete m_Player, m_Camera, m_skybox, m_dynamicsWorld;
    for (int i = 0; i < m_Objects.size(); i++)
        delete m_Objects[i];
    
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
        if (!Obj.second.first->loaded())
            Obj.second.first->Load();
        glm::mat4 model(1.0f);
        if (Obj.first == "Player") // CREAR OBJETO COCHE
        {
            m_Player = new Player(p, m_dynamicsWorld);
            // This could be at the constructor
            m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
            m_renderer->setModelMatrix(Obj.first, model);
            if (m_renderer->getNSpotLights() >= 2) //TODO: study a way to avoid magic numbers
                m_Player->setLights(m_renderer->getSpotLight(0), m_renderer->getSpotLight(1));
        }
        else if (Obj.first.substr(0, 4) == "Coin") { // CREAR OBJETO MONEDA
            m_Coins.push_back(new Coin(Obj.second.first, m_dynamicsWorld, Obj.second.second, Obj.first));
            m_Coins.back()->getGhostObject()->getWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
            m_renderer->setModelMatrix(Obj.first, model);
        }
        else if(Obj.first.substr(0, 5) != "Wheel") // CREAR RESTO DE OBJETOS MENOS RUEDAS
        {
            m_Objects.push_back(new Object(Obj.second.first, m_dynamicsWorld, Obj.second.second));
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

void Game::img_loader() {
    int width, height;
    int channels;
    unsigned char* pixels = stbi_load("../../../Assets/Imagenes/LogoJuego.jpg", &width, &height, &channels, 4);

    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(m_Window, 1, images);
}


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
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        m_renderer->cycleDirLight();
        m_renderer->cycleSky();
    }

    m_Player->InputMethod(key, action);
}

void Game::Actualizar(float deltaTime)
{
    // Physics
    m_dynamicsWorld->stepSimulation(deltaTime, 2);

    // Loop for the ghost coin!
    for (auto coin = m_Coins.begin(); coin != m_Coins.end();) {
        bool coinCollected = false;
        for (int i = 0; i < (*coin)->getGhostObject()->getNumOverlappingObjects(); i++) {
            btCollisionObject* obj = (*coin)->getGhostObject()->getOverlappingObject(i);
            // Comprueba si el objeto que se superpone es el que te interesa (por ejemplo, el jugador)
            if (obj == m_Player->vehicle->getRigidBody()) {
                std::cout << "COLISION!" << std::endl;
                // El jugador ha recogido la moneda, así que la eliminamos
                m_dynamicsWorld->removeCollisionObject((*coin)->getGhostObject());
                m_renderer->RemoveModel((*coin)->m_id);
                coin = m_Coins.erase(coin);
                coinCollected = true;
                break;
            }
        }
        if (!coinCollected) {
            ++coin;  // Solo incrementamos el iterador si no hemos recogido la moneda
        }
    }

    // Animations
    if (m_sinTime >= 2.0f * glm::pi<float>())
        m_sinTime = 0.0f;

	// Perform Animations
    for (auto coin : m_Coins) {
		performJumpAndSpin(coin->m_id, m_sinTime, m_sinTime + deltaTime);    
    }
    // Update sinTime
	m_sinTime += deltaTime;
}

void Game::Render()
{
    {
		glm::mat4 model(1.0f);
		m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
        // model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5)); CAMBIAR TAMAÑO COCHE
        //std::cout << "Player position:" << m_Player->vehicle->getChassisWorldTransform().getOrigin().getY() << std::endl;
        m_renderer->setModelMatrix("Player", model);
        m_Player->updatePlayerData();
    }

    for (int i = 0; i < 4; i++) {
        glm::mat4 model(1.0f);
        m_Player->vehicle->getWheelTransformWS(i).getOpenGLMatrix(glm::value_ptr(model));
        // model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5)); CAMBIAR TAMAÑO COCHE
        m_renderer->setModelMatrix("Wheel" + std::to_string(i), model);
    }

    // Update camera
    m_Camera->followPlayer();

    glm::mat4 projection = glm::perspective(glm::radians(m_Camera->FOV), (float)m_SCR_WIDTH / (float)m_SCR_HEIGHT, c_near, c_far);

    m_renderer->RenderEverything(*m_Camera, projection);
}

// Function to perform jump and spin animation
void Game::performJumpAndSpin(std::string id, float time1, float time2) {

    glm::mat4 modelMatrix = m_renderer->getModel(id).second;
    // Calculate jump animation
    float jump = jumpHeight * (sin(jumpDuration * time2) - sin(jumpDuration * time1));
    // std::cout << jump << std::endl;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, jump, 0.0f));

    // Calculate spin animation
    float rotation = (time2 - time1)*spinSpeed;
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    m_renderer->setModelMatrix(id, modelMatrix);
}
#pragma endregion
