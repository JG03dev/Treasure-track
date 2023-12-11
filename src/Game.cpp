#include "Game.h"


#pragma region PUBLIC_METHODS



void Game::StartGame() {

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

void Timer(float deltaTime, ImFont* myFont,  static float timer) {
    
}


void Game::Run()
{
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    MySoundEffects sound;

    img_loader();

    //HABRIA QUE PONERLO EN UNA FUNCION A PARTE...
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../../src/Graficos/NFS.ttf", 15.0f);  // Cambia el nombre del archivo y el tamaño según tus necesidades
    ImFont* myFont = io.Fonts->Fonts[0];

    // Inicialización de ImGui para GLFW y OpenGL
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    static float timer = 15.0f;

    while (!glfwWindowShouldClose(m_Window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sound.PlaySound(deltaTime);
        sound.PlayMusic(deltaTime);

        // input: Manejar entrada de usuario
        // -----
        glfwPollEvents();

        //Actualizar Informacion (Mover coordenadas)
        Actualizar(deltaTime);

        //Renderizar
        Render();

        //PONER TODO ESTO DE AQUÍ ABAJO EN UNA FUNCION A PARTE HACE QUE EL TEMPORIZADOR NO VAYA, PQ??
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
#pragma endregion
