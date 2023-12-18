#include "Game.h"
#include <SOIL2/SOIL2.h>

#pragma region DESTRUCTORS

Game::~Game()
{
    DeleteCurrentGame();
    if (m_ui != NULL) delete m_ui;
    if (m_Camera != NULL) delete m_Camera;
}

void Game::DeleteCurrentGame()
{
    if (m_renderer != NULL) { 
        delete m_renderer; 
        m_renderer = nullptr; }
    if (m_Player != NULL) { m_Player->DestroyObject(m_dynamicsWorld); delete m_Player; m_Player = nullptr; }
    for (int i = 0; i < m_Objects.size(); i++) {
        if (m_Objects[i] != NULL) { m_Objects[i]->DestroyObject(m_dynamicsWorld); delete m_Objects[i]; m_Objects[i] = nullptr; }
    }
    m_Objects.clear();

    for (int i = 0; i < m_Coins.size(); i++) {
        if (m_Coins[i] != NULL) { m_Coins[i]->DestroyObject(m_dynamicsWorld); delete m_Coins[i]; m_Coins[i] = nullptr; }
    }
    m_Coins.clear();

    if (m_dynamicsWorld != NULL) {
        for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                while (body->getNumConstraintRefs())
                {
                    btTypedConstraint* constraint = body->getConstraintRef(0);
                    m_dynamicsWorld->removeConstraint(constraint);
                    delete constraint;
                }
                delete body->getMotionState();
                m_dynamicsWorld->removeRigidBody(body);
            }
            else
            {
                m_dynamicsWorld->removeCollisionObject(obj);
            }
            delete obj;
        }
        btConstraintSolver* constraintSolver = m_dynamicsWorld->getConstraintSolver();
        btBroadphaseInterface* broadphaseInterface = m_dynamicsWorld->getBroadphase();
        btDispatcher* collisionDispatcher = this->m_dynamicsWorld->getDispatcher();

        delete m_dynamicsWorld; m_dynamicsWorld = nullptr;
        delete constraintSolver;
        delete broadphaseInterface;
        delete collisionDispatcher;
    }

    if (m_sound != NULL) {
        delete m_sound; m_sound = nullptr;
    }
    m_PreviousSpeed = 0; // This is to avoid the crashing sound when restarting the game
}

#pragma endregion

#pragma region HANDLERS

void Game::HandleMainMenu()
{
    UIEvents e = m_ui->DrawAndPollEvents(DMainMenu);

    switch (e)
    {
    case Start_Game:
        m_currentState = ModeSelector;
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
    
    if (m_progressBar > 0.4f && m_progressBar < 0.45f) 
    {
        InitializeSound();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        m_progressBar = 0.46f;
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
    m_ui->DrawAndPollEvents(DLoadScreen, m_progressBar);

    // Once progress bar is finished start the game
    if (m_progressBar >= 1.0f) {
        m_currentState = InGame;
        m_lastFrame = static_cast<float>(glfwGetTime());
        m_progressBar = 0.0f;
        m_timer = 200.0f;
        m_coinsCollected = 0;
    }
}

void Game::HandlePause()
{
    switch (m_ui->DrawAndPollEvents(DPauseMenu)) {
    case Resume:
        m_currentState = InGame;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        InitializeInput();  // Pause menu somehow disables input or something like that, so I initialize it again. It works
        break;
    case Return:
        m_currentState = MainMenu;
        DeleteCurrentGame();
        break;
    case Exit:
        glfwSetWindowShouldClose(m_Window, true);
        break;
	}
}

void Game::HandleModeSelector()
{
    //Make UI Draw Mode
    switch(m_ui->DrawAndPollEvents(DMenuModeSelection)){
    case CoinMode:
    // Coin mode
        m_coinMode = true;
        m_currentState = TimeSelector;
        break;
    case FreeMode:
    // No coins
        m_coinMode = false;
		m_currentState = TimeSelector;
        break;
    }
}

void Game::HandleTimeSelection()
{
	//Make UI Draw Mode
	switch (m_ui->DrawAndPollEvents(DMenuTimeSelection)) {
	case Day:
		// Set day
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_initSky = 0;
        m_currentState = Loading;
		break;
	case Afternoon:
		// Set afternoon
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_initSky = 1;
		m_currentState = Loading;
		break;
	case Night:
		// Set night
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_initSky = 2;
		m_currentState = Loading;
		break;
	}
}

void Game::HandleGameOver()
{
    switch (m_ui->DrawAndPollEvents(DEndScreen, 0, 0, 0, m_coinsCollected,m_totalCoins)) {
    case Return:
        m_currentState = MainMenu;
        DeleteCurrentGame();
        break;
    }
}

#pragma endregion

#pragma region MAIN-LOOP

int Game::Start()
{
    InitializeInput();
    m_ui = new UIHandler(m_Window);
    img_loader();

    // Main menu loop
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

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
        case GameState::Paused:
            HandlePause();
            break;
        case GameState::ModeSelector:
            HandleModeSelector();
            break;
        case GameState::TimeSelector:
            HandleTimeSelection();
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
    m_progressBar += 1.0f / 250.0f;
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
    m_renderer = new Renderer("../../../Assets/Objects.json", m_SCR_WIDTH, m_SCR_HEIGHT, m_initSky, m_initSky);
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
        }
        else if (Obj.first.substr(0, 4) == "Coin") { // CREAR OBJETO MONEDA
            if(m_coinMode)
            {
                m_Coins.push_back(new Coin(Obj.second.first, m_dynamicsWorld, Obj.second.second, Obj.first));
                m_Coins.back()->getGhostObject()->getWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
                m_renderer->setModelMatrix(Obj.first, model);
            } 
            else
            {
                m_renderer->RemoveModel(Obj.first);
            }
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
    m_Camera->setTarget(m_Player); 
    m_totalCoins = m_Coins.size();
}

void Game::InitializeSound()
{
    m_sound = new MySoundEffects();
}

void Game::InitializeInput() {
    glfwSetWindowUserPointer(GetWindow(), this);

    glfwSetKeyCallback(GetWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            static_cast<Game*>(glfwGetWindowUserPointer(window))->KeyCallback(window, key, scancode, action, mods);
        });

    glfwSetCursorPosCallback(GetWindow(), [](GLFWwindow* window, double xpos, double ypos)
        {
            static_cast<Game*>(glfwGetWindowUserPointer(window))->MouseCallback(window, xpos, ypos);
        });

    glfwSetScrollCallback(GetWindow(), [](GLFWwindow* window, double xoffset, double yoffset)
        {
            static_cast<Game*>(glfwGetWindowUserPointer(window))->ScrollCallback(window, xoffset, yoffset);
        });
}

int Game::InitializeWindow()
{
    // glfw window creation
    // --------------------
    m_Window = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "City Driver Simulator", NULL, NULL);
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
    m_sound->PlayMusic(m_deltaTime);
    m_sound->PlaySound(m_deltaTime);

    //Actualizar Informacion (Mover coordenadas)
    Actualizar(m_deltaTime);
    m_PreviousSpeed = m_Player->vehicle->getCurrentSpeedKmHour();

    //Renderizar
    Render();

    ///HUD Logic

    // Incrementar el temporizador
    if (m_timer > 0.0f)
    {
        m_timer -= m_deltaTime;
    }

    float carSpeed = m_Player->vehicle->getCurrentSpeedKmHour();
    float carSpeedRad = carSpeed * (M_PI / 180.0f);
    float rotationAngle = carSpeedRad * 1.8f;

    UIFlags f = m_coinMode ? static_cast<UIFlags>(DHUD | DCoinHUD) : DHUD;

    m_ui->DrawAndPollEvents(f, m_timer, carSpeed, rotationAngle, m_coinsCollected, m_totalCoins);
    
    if (m_coinMode && (m_timer <= 0 || m_coinsCollected == m_totalCoins)) {
        m_currentState = GameOver;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        delete m_ui;
        m_ui = new UIHandler(m_Window);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (m_currentState)
    {
    case MainMenu:
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        break;
    case ModeSelector:
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            m_currentState = GameState::MainMenu;
        }
        break;
    case TimeSelector:
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            m_currentState = GameState::ModeSelector;
        }
        break;
    case Paused:
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            m_currentState = GameState::InGame;
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        break;
    case InGame:
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_currentState = GameState::Paused;
            delete m_ui;
            m_ui = new UIHandler(m_Window);
            // Round up camera front to avoid pause view glitches
            m_Camera->front = glm::vec3(float(int(m_Camera->front.x)), float(int(m_Camera->front.y)), float(int(m_Camera->front.z)));
        }

        if (GLFW_KEY_C == key && action == GLFW_PRESS)
        {
            m_Camera->changeCamera();
        }

        // Luces
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            // Cambiar el estado de las luces
            m_renderer->getSpotLight(0)->Toggle();
            m_renderer->getSpotLight(1)->Toggle();
        }
        if (m_renderer->getNSpotLights() >= 2 && m_renderer->getSpotLight(0)->isActive() && m_renderer->getSpotLight(1)->isActive()) //TODO: study a way to avoid magic numbers
            m_Player->setLights(m_renderer->getSpotLight(0), m_renderer->getSpotLight(1));
        else
            m_Player->setLights(nullptr, nullptr);

        m_Player->InputMethod(key, action);
        break;
    default:
        break;
    }
}

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos) {

    static bool  firstMouse = true;
    static float lastX = m_SCR_WIDTH / 2;
    static float lastY = m_SCR_HEIGHT / 2;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coord range from buttom to top
    lastX = xpos;
    lastY = ypos;

    m_Camera->processMouse(xoffset, yoffset);
}

void Game::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    m_Camera->processScroll(yoffset);
}

void Game::Actualizar(float deltaTime)
{
    // Physics
    m_dynamicsWorld->stepSimulation(deltaTime, 2);

    //Sonido choque
    if (m_Player->vehicle->getCurrentSpeedKmHour() < (m_PreviousSpeed - 10)) {
        m_sound->Choque();
    }

    // Loop for the ghost coin!
    for (auto coin = m_Coins.begin(); coin != m_Coins.end();) {
        bool coinCollected = false;
        for (int i = 0; i < (*coin)->getGhostObject()->getNumOverlappingObjects(); i++) {
            btCollisionObject* obj = (*coin)->getGhostObject()->getOverlappingObject(i);
            // Comprueba si el objeto que se superpone es el que te interesa (por ejemplo, el jugador)
            if (obj == m_Player->vehicle->getRigidBody()) {
                std::cout << "COLISION!" << std::endl;
                // El jugador ha recogido la moneda, asi que la eliminamos
                m_sound->PlayCoinSound();
                m_coinsCollected++;
                std::cout << "Monedas recogidas: " << m_coinsCollected << std::endl;
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
        // model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5)); CAMBIAR TAMANO COCHE
        //std::cout << "Player position:" << m_Player->vehicle->getChassisWorldTransform().getOrigin().getY() << std::endl;
        m_renderer->setModelMatrix("Player", model);
        m_Player->updatePlayerData();
    }

    for (int i = 0; i < 4; i++) {
        glm::mat4 model(1.0f);
        m_Player->vehicle->getWheelTransformWS(i).getOpenGLMatrix(glm::value_ptr(model));
        // model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5)); CAMBIAR TAMANO COCHE
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
