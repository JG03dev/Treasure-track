#include "Game.h"
#include <SOIL2/SOIL2.h>

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
        if (Obj.first == "Player") // CREAR OBJETO COCHE
        {
            m_Player = new Player(p, m_dynamicsWorld);
            // This could be at the constructor
            m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
            m_renderer->setModelMatrix(Obj.first, model);
            /*
            if (m_renderer->getNSpotLights() >= 2) //TODO: study a way to avoid magic numbers
                m_Player->setLights(m_renderer->getSpotLight(0), m_renderer->getSpotLight(1));*/
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

void Game::InitializeSound()
{
    m_sound = new MySoundEffects();
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

GLuint LoadTexture(const char* filepath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return textureID;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return 0;
    }
}

void Game::DisplayLoadingScreen()
{
    // Load your jpg image here
    GLuint textureID = LoadTexture("../../../Assets/Imagenes/LoadingScreenFoto.png");

    // Create a quad for the image to be displayed on
    float vertices[] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Create Vertex Buffer Object (VBO), Vertex Array Object (VAO), and Element Buffer Object (EBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Render the image
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Assuming you have an instance of the Shader class called 'shader'
    Shader shader;
    glUseProgram(shader.getProgramID());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Swap the buffers
    glfwSwapBuffers(m_Window);
}

GLuint LoadTexture2(const char* path) {
    GLuint textureID;
    int width, height, channels;

    // Cargar la textura con SOIL2
    unsigned char* image = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_RGBA);

    if (!image) {
        std::cerr << "Error cargando la textura desde " << path << std::endl;
        return 0;
    }

    // Generar un ID de textura
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Configurar la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Liberar memoria de la imagen cargada
    SOIL_free_image_data(image);

    // Configurar parámetros de textura (puedes ajustarlos según tus necesidades)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

int rotation_start_index;
void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);


    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

ImVec2 operator-(const ImVec2& l, const ImVec2& r) { return{ l.x - r.x, l.y - r.y }; }
ImVec2 operator+(const ImVec2& l, const ImVec2& r) { return{ l.x + r.x, l.y + r.y }; }

ImVec2 sin_cos_interval_min(0.735723, -0.727309);
ImVec2 sin_cos_interval_max(0.68631, 0.677282);
void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float desfase = M_PI / 1.35f;
    float s = sin(rad - desfase), c = cos(rad - desfase);

    center = ImRotate(center, s, c) - center;
    if (s >= sin_cos_interval_min.x && s <= sin_cos_interval_max.x &&
        c >= sin_cos_interval_min.y && c <= sin_cos_interval_max.y)
    {
        std::cout << "beep beep" << std::endl;
    }
    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void ImRotateEnd_MinMax(float rad, int v, ImVec2 center = ImRotationCenter())
{
    float s, c;
    if (v == 0)
    {
        s = -0.735723, c = -0.677282;
    }
    else
    {
        s = -0.68631, c = 0.727309;
    }

    center = ImRotate(center, s, c) - center;
    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}


void Game::Run()
{
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    MySoundEffects sound;
    
    //HABRIA QUE PONERLO EN UNA FUNCION A PARTE...
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../../src/Graficos/NFS.ttf", 15.0f);  // Cambia el nombre del archivo y el tama�o seg�n tus necesidades
    ImFont* myFont = io.Fonts->Fonts[0];

    // Inicializaci�n de ImGui para GLFW y OpenGL
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    static float timer = 100.0f;

    // Cargar texturas
    GLuint textureSpeedometer = LoadTexture2("../../../src/speedometer.png");
    GLuint texturePointer = LoadTexture2("../../../src/pointer.png");

    // Verifica que los IDs de textura sean v�lidos antes de usarlos en la renderizaci�n
    if (textureSpeedometer == 0 || texturePointer == 0) {
        // Manejo de error, por ejemplo, salir del programa o mostrar un mensaje de error
        std::cerr << "Error cargando texturas." << std::endl;
        exit;
    }

    while (!glfwWindowShouldClose(m_Window))
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

        //PONER TODO ESTO DE AQU� ABAJO EN UNA FUNCION A PARTE HACE QUE EL TEMPORIZADOR NO VAYA, PQ??
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::PushFont(myFont);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(250, 50), ImGuiCond_Always);

        //Desactivar las decoraciones de la ventana
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        //Quitar el fondo transparente
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        ImGui::Begin("Temporizador", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowFontScale(2.0f);
        if (timer > 25.0f)
        {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Tiempo: %.2f", timer);
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Tiempo: %.2f", timer);
        }

        // Incrementar el temporizador
        if (timer > 0.0f)
        {
            timer -= deltaTime;
        }
        ImGui::End();

        //Actualizar Informacion (Mover coordenadas)
        Actualizar(deltaTime);

        ImGui::SetNextWindowPos(ImVec2(900, 400), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(450, 500), ImGuiCond_Always);
        ImGui::Begin("Speedometer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
        // Mostrar la imagen del veloc�metro
        ImVec2 speedometerSize(300.0f, 300.0f);  // Ajusta seg�n sea necesario
        ImGui::Image((void*)(intptr_t)textureSpeedometer, speedometerSize);
        ImGui::End();


        float carSpeed = m_Player->vehicle->getCurrentSpeedKmHour();
        float carSpeedRad = carSpeed * (M_PI / 180.0f);
        float rotationAngle = carSpeedRad * 1.8f;

        ImGui::SetNextWindowPos(ImVec2(900, 400), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(450, 500), ImGuiCond_Always);
        ImGui::Begin("pointer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

        // Mostrar la imagen del indicador
        ImVec2 pointerSize(300.0f, 300.0f); // Ajusta seg�n sea necesario
        // Dibuja la imagen sin rotaci�n
        ImRotateStart();
        ImGui::Image((void*)(intptr_t)texturePointer, pointerSize);
        if (carSpeed > 150)
        {
            ImRotateEnd_MinMax(-rotationAngle, 1);
        }
        else
        {
            if (carSpeed < -1)
            {
                ImRotateEnd_MinMax(-rotationAngle, 0);
            }
            else
            {
                ImRotateEnd(-rotationAngle);
            }
        }
        ImGui::End();

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();
        ImGui::PopFont();
        // Renderizar la interfaz de usuario de ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
}

void Game::Actualizar(float deltaTime)
{
    this->m_Player->lastForward = this->m_Player->getCarForward();
    this->m_Player->lastQuaternion = this->m_Player->getCarRotation();

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
                // El jugador ha recogido la moneda, as� que la eliminamos
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

    // Actualizar el contador de monedas en la interfaz
    ImGui::SetNextWindowPos(ImVec2(10, 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, 50), ImGuiCond_Always);

    ImGui::Begin("Contador de Monedas", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(2.0f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Monedas: %d / %d", m_coinsCollected, m_Coins.size() + m_coinsCollected);
    ImGui::End();
}

void Game::Render()
{
    {
		glm::mat4 model(1.0f);
		m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));
        // model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5)); CAMBIAR TAMA�O COCHE
        //std::cout << "Player position:" << m_Player->vehicle->getChassisWorldTransform().getOrigin().getY() << std::endl;
        m_renderer->setModelMatrix("Player", model);
        m_Player->updatePlayerData();
    }

    for (int i = 0; i < 4; i++) {
        glm::mat4 model(1.0f);
        m_Player->vehicle->getWheelTransformWS(i).getOpenGLMatrix(glm::value_ptr(model));
        // model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5)); CAMBIAR TAMA�O COCHE
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
    // std::cout << jump << std::endl;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, jump, 0.0f));

    // Calculate spin animation
    float rotation = (time2 - time1)*spinSpeed;
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    m_renderer->setModelMatrix(id, modelMatrix);
}
#pragma endregion
