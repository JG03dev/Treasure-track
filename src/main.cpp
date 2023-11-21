#include <vector>

#include "btBulletDynamicsCommon.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "freetype/freetype.h"
#include FT_FREETYPE_H

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <AL/al.h>
#include <AL/alc.h>

#include "Mesh.h"
#include "Shader.h"
#include "text_fonts_glyphs.h"

#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "PhysicsEngine.h"

#include "SoundDevice.h"
#include <iostream>
#include "MusicBuffer.h"
#include "SoundEffectsLibrary.h"
#include "SoundEffectsPlayer.h"
#include "MainLoop/MainLoop.h"  // for looping
#include <Windows.h>  // for keyboard press

const float toRadians = 3.14159265f / 180.0f;

Renderer* Renderer::renderer = NULL;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "../../../Shaders/shader.vert";
// Fragment shader
static const char* fShader = "../../../Shaders/shader.frag";

//sonidos
uint32_t sound1, sound2;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

Shader* CreateShader()
{
    auto* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    return shader1;
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

int main(int argc, char* argv[])
{
    Renderer* renderer = Renderer::getInstance();
    renderer->startWindow();    
    
    PhysicsEngine pE;
    renderer->addShader(CreateShader());

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);
    camera.setTarget(pE.player);

    renderer->addCamera(&camera);

    if (initialize_libraries() < 0)
        return -1;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

    SoundDevice::init();
    
    static SoundEffectsPlayer effectsPlayer1;
    static uint32_t sound1 = SE_LOAD("../../../music/sounds/arranque.wav");
    static MusicBuffer myMusic("../../../music/coconut.wav");
    bool estado = false;

    // compilar shader texto
    const char* vert_shader_text = "../../../Shaders/shader_text.vert";
    const char* frag_shader_text = "../../../Shaders/shader_text.frag";

    Shader text_shader(vert_shader_text, frag_shader_text);
    text_shader.UseShader();

    // inicializar freetype (texto)
    FT_Library free_type;
    FT_Error error_code = FT_Init_FreeType(&free_type);
    if (error_code)
    {
        std::cout << "\n   Error code: " << error_code << " --- " << "An error occurred during initialising the FT_Library";
        int keep_console_open;
        std::cin >> keep_console_open;
    }
    Text text_object2(free_type, 1280, 720, "01234567890Get Rady.Timr:owns&ClBgfb");
    text_object2.create_text_message("Get Ready... Timer: 000", 0, 0, "../../../Text Fonts/arialbi.ttf", 50, true);

    glUniform1i(glGetUniformLocation(text_shader.ID, "alphabet_texture"), 31);

    //color letras
    glm::vec3 _RGB(10.0f, 120.0f, 105.0f);
    unsigned int font_colour_loc = glGetUniformLocation(text_shader.ID, "font_colour");
    glUniform3fv(font_colour_loc, 1, glm::value_ptr(_RGB));

	while (!renderer->mainWindow->getShouldClose())
	{
        myMusic.UpdateBufferStream();
        GLfloat now = (GLfloat) glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        static float musiccontrolcooldown = 1;
        musiccontrolcooldown += deltaTime;
        if (musiccontrolcooldown > 0 && GetKeyState('Q') & 0x8000)
        {          
            if (myMusic.isPlaying())
            {
                myMusic.Pause();
            }
            else
            {
                myMusic.Play();
            }
            _sleep(100);
            musiccontrolcooldown = 0;
        } else if (musiccontrolcooldown > 0 && GetKeyState('Z') & 0x8000)
        {
            if (myMusic.isPlaying())
            {
                myMusic.Pause();

                static MusicBuffer myMusic("../../../music/cars.wav");
                myMusic.Play();
            }
            else
            {
                static MusicBuffer myMusic("../../../music/cars.wav");
            }
            _sleep(100);    
            musiccontrolcooldown = 0;
        }

        //imprimir texto
        glDisable(GL_DEPTH_TEST);
        text_object2.draw_alphabets();
        glEnable(GL_DEPTH_TEST);
        
        
        pE.stepSimulation(deltaTime);
		glfwPollEvents();

        pE.player->keyboardCallback(renderer->getKeys(), deltaTime, 1, estado);
        if (estado)
        {
            
            estado = false;
        }

        renderer->updateCameraPos();
        renderer->updateCameraRotation();

        renderer->clearWindow();

        renderer->render(pE.groundRB, pE.player->vehicle->getChassisWorldTransform());
	}

	return 0;
}

