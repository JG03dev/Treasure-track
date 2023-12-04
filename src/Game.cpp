#include "Game.h"
#include "text_fonts_glyphs.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h> // https://freetype.org/freetype2/docs/tutorial/step1.html#section-1
#include FT_FREETYPE_H

#include <glm/glm.hpp> // Include all GLM core.	
#include <glm/gtc/matrix_transform.hpp> // Specific extensions.
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <fstream> // Used in "shader_configure.h" to read the shader text files.
#include <map>
#include <string>

#pragma region PUBLIC_METHODS


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;


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
                            "../../../Shaders/shader_text.vert", "../../../Shaders/shader_text.frag",
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

    //m_Objects.push_back(new Object("../../../Assets/town/town.obj", "town", m_dynamicsWorld, 4.0f, 256));

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

    m_renderer->RenderEverything(m_Camera->getViewMatrix(), projection, *m_Camera);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// render line of text
// -------------------
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    shader.use();
    glUniform3f(glGetUniformLocation(shader._ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma endregion
