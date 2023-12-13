#include "Game.h"


#pragma region PUBLIC_METHODS
Game::~Game()
{
    if (m_ui != NULL) delete m_ui;
    if (m_renderer != NULL) delete m_renderer;
    if (m_Player != NULL) delete m_Player;
    if (m_Camera != NULL) delete m_Camera;
    if (m_dynamicsWorld != NULL) delete m_dynamicsWorld;
    for (int i = 0; i < m_Objects.size(); i++)
        if (m_Objects[i] != NULL) delete m_Objects[i];
    if (m_sound != NULL) delete m_sound;
}
void Game::HandleMainMenu()
{
    UIEvents e = m_ui->DrawAndPollEvents(Main_Menu);

    switch (e)
    {
    case Start_Game:
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_currentState = Loading;
        break;
    case Help: //Or settings
        // Call help or setting function
        std::cout << "WASD to move, SHIFT for TURBO" << std::endl;
        break;
    case Exit:
        glfwSetWindowShouldClose(m_Window, true);
    default:
        break;    
    }
}

GLFWwindow* Game::CreateSharedGLFWWindow() {
    // Create and configure a GLFW window with a shared OpenGL context
    // Note: This is a simplified example; adjust based on your GLFW setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* sharedContextWindow = glfwCreateWindow(1, 1, "SharedContextWindow", nullptr, nullptr);
    if (!sharedContextWindow) {
        // Handle window creation failure
        return nullptr;
    }

    return sharedContextWindow;
}
void Game::HandleLoading()
{
    UpdateProgressBar();
    
    if (m_progressBar > 0.25f && m_progressBar < 0.3f)
    {
        InitializePhysics();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        m_progressBar = 0.31f;
        m_ui->cycleLoadingTexts();
    }
    
    if (m_progressBar > 0.5f && m_progressBar < 0.6f) 
    {
        InitializeSound();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        m_progressBar = 0.61f;
        m_ui->cycleLoadingTexts();
    }
    
    if (m_progressBar > 0.8f && m_progressBar < 0.85f)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        m_progressBar = 0.86f;
        m_ui->cycleLoadingTexts();
    }

    if (m_progressBar > 0.95f)
    {
        InitializeGraphics();
        m_progressBar = 1.0f;
    }

    // Update progress bar with easing
    m_ui->DrawAndPollEvents(Load_Screen, m_progressBar);

    // Once progress bar is finished start the game
    if(m_progressBar >= 1.0f)
        m_currentState = InGame;
}

void Game::HandleGameOver()
{
}

int Game::Start()
{
    m_ui = new UIHandler(m_Window);
    img_loader();

    // Main menu loop
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();

        switch (m_currentState) {
        case GameState::MainMenu:
            HandleMainMenu();
            break;
        case GameState::Loading:
            HandleLoading();
            break;
        case GameState::InGame:
            Run();
            break;
        case GameState::GameOver:
            HandleGameOver();
            break;
        }

        glfwSwapBuffers(m_Window);
    }
    return 0;
}
#pragma endregion

#pragma region PRIVATE_METHODS_INITIALIZERS

void Game::UpdateProgressBar()
{// TODO: try different easing functions
    m_progressBar += 1.0f / 25000.0f;
}

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
        if (Obj.first == "Player")
        {
            m_Player = new Player(p, m_dynamicsWorld);
            // This could be at the constructor
            m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
            m_renderer->setModelMatrix(Obj.first, model);
            if (m_renderer->getNSpotLights() >= 2) //TODO: study a way to avoid magic numbers
                m_Player->setLights(m_renderer->getSpotLight(0), m_renderer->getSpotLight(1));
        }
        else if(Obj.first.substr(0, 5) != "Wheel")
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

void Game::InitializeSound()
{
    m_sound = new MySoundEffects();
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
    unsigned char* pixels = stbi_load("../../../src/s2.jpg", &width, &height, &channels, 4);

    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(m_Window, 1, images);
}


void Game::Run()
{
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        m_sound->PlaySound(m_deltaTime);

        //Actualizar Informacion (Mover coordenadas)
        Actualizar(m_deltaTime);

        //Renderizar
        Render();
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

    // Animations
    if (m_sinTime >= 2.0f * glm::pi<float>())
        m_sinTime = 0.0f;

    // Perform Animations
    performJumpAndSpin("Test-Coin", m_sinTime, m_sinTime+deltaTime);
    
    // Update sinTime
    m_sinTime += deltaTime;
}

void Game::Render()
{
    {
		glm::mat4 model(1.0f);
		m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
        //std::cout << "Player position:" << m_Player->vehicle->getChassisWorldTransform().getOrigin().getY() << std::endl;
        m_renderer->setModelMatrix("Player", model);
        m_Player->updatePlayerData();
    }

    for (int i = 0; i < 4; i++) {
        glm::mat4 model(1.0f);
        m_Player->vehicle->getWheelTransformWS(i).getOpenGLMatrix(glm::value_ptr(model));
        m_renderer->setModelMatrix("Wheel" + std::to_string(i), model);
    }

    // Update camera
    m_Camera->followPlayer();

    glm::mat4 projection = glm::perspective(glm::radians(m_Camera->FOV), (float)m_SCR_WIDTH / (float)m_SCR_HEIGHT, c_near, c_far);

    m_renderer->RenderEverything(*m_Camera, projection);
}

void Game::LoadScreen() {
}

// Function to perform jump and spin animation
void Game::performJumpAndSpin(std::string id, float time1, float time2) {

    glm::mat4 modelMatrix = m_renderer->getModel(id).second;
    // Calculate jump animation
    float jump = jumpHeight * (sin(jumpDuration * time2) - sin(jumpDuration * time1));
    std::cout << jump << std::endl;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, jump, 0.0f));

    // Calculate spin animation
    float rotation = (time2 - time1)*spinSpeed;
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    m_renderer->setModelMatrix("Test-Coin", modelMatrix);
}
#pragma endregion
