#include "Game.h"

Skybox Game::loadSkybox()
{
    //Load Faces
    std::vector<std::string> faces =
    { "../../../Assets/skybox/right.jpg",
        "../../../Assets/skybox/left.jpg",
        "../../../Assets/skybox/top.jpg",
        "../../../Assets/skybox/bottom.jpg",
        "../../../Assets/skybox/front.jpg",
        "../../../Assets/skybox/back.jpg"
    };
    //Load Skybox with shaders
    return Skybox(faces, "../../../Shaders/skybox.VERT", "../../../Shaders/skybox.FRAG");
}

void Game::Initializer()
{
    m_skybox = loadSkybox();

    // settings
    m_blinn = true;
    m_blinnKeyPressed = false;

    // camera
    m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f)); 
    m_lastX = m_SCR_WIDTH / 2.0f;
    m_lastY = m_SCR_HEIGHT / 2.0f;
    m_firstMouse = true;

    //timer
    m_deltaTime = 0.0f;
    m_lastFrame = 0.0f;

    // lighting info
    m_lightPos = glm::vec3(20.0f, 0.0f, 0.0f);

    // window
    InitializeWindow();
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
  /*  glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);*/

    // tell GLFW to capture our mouse
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum err = glewInit();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
}

void Game::AddShader(const char* s1, const char* s2)
{
    m_Shaders.push_back(new Shader(s1, s2));
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


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

    //W
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Player->InputMethod(GLFW_KEY_W, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        m_Player->InputMethod(GLFW_KEY_W, false);

    //S
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Player->InputMethod(GLFW_KEY_S, true);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        m_Player->InputMethod(GLFW_KEY_S, false);

    //A
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Player->InputMethod(GLFW_KEY_A, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        m_Player->InputMethod(GLFW_KEY_A, false);

    //D
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Player->InputMethod(GLFW_KEY_D, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        m_Player->InputMethod(GLFW_KEY_D, false);

    //Ctrl
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_Player->InputMethod(GLFW_KEY_LEFT_CONTROL, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
        m_Player->InputMethod(GLFW_KEY_LEFT_CONTROL, false);
}

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
		processInput(m_Window);

		//Actualizar Informacion (Mover coordenadas)
		Actualizar();

		//Renderizar
		Render();
	}
}

void Game::Actualizar()
{
    m_dynamicsWorld->stepSimulation(m_deltaTime, 2);
}

void Game::Render()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    m_Shaders[1]->use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)m_SCR_WIDTH / (float)m_SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shaders[1]->setMat4("projection", projection);
    m_Shaders[1]->setMat4("view", view);

    // set light uniforms
    m_Shaders[1]->setVec3("viewPos", m_Camera->Position);
    m_Shaders[1]->setVec3("lightPos", m_lightPos);
    m_Shaders[1]->setInt("blinn", m_blinn);

    for (int i = 0; i < m_Objects.size(); i++)
    {
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);

        btTransform t = btTransform();
        m_Objects[i]->rb->getMotionState()->getWorldTransform(t);
        t.getOpenGLMatrix(glm::value_ptr(model));

        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        m_Shaders[1]->setMat4("modelMatrix", glm::mat4(1.0f));
        m_Objects[i]->Draw(*m_Shaders[1]);
    }

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);

    btTransform t = btTransform();
    m_Player->vehicle->getChassisWorldTransform().getOpenGLMatrix(glm::value_ptr(model));

    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    m_Shaders[1]->setMat4("modelMatrix", glm::mat4(1.0f));
    m_Player->Draw(*m_Shaders[1]);

    //Skybox
    m_skybox.drawSkybox('Y', projection, view);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}