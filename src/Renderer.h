/*
* Classe Renderer: Esta classe se encargara de hacer las funciones de renderizacion de objetos, luces y sombras de la escena,
* Esta evita tener un main o una classe game de 4k lineas por el exceso de funciones que no pertenecen estar en un archivo tan
* general
*/

#include "include/stdafx.h"
#include "globals.h"

// Renderizacion General
#include "General/Shader.h"
#include "General/Skybox.h"
#include "General/Camera.h"
#include "Modelo/Model.h"

// Iluminado
#include "Luces/DirectionalLight.h"
#include "Luces/PointLight.h"
#include "Luces/SpotLight.h"
#include "Sombras/ShadowMap.h"
#include "Sombras/OmniShadowMap.h"

class Renderer
{
public:
	Renderer() : skybox(NULL), sObject(NULL), sSkybox(NULL), sDirShadow(NULL), sOmniShadow(NULL),
		mainLight(NULL) {}
	
	// TODO: Complete class to cover up all cases

	Renderer(Shader* sObject, Shader *sSky, Shader*sDirSha, Shader *sOmniSha, Skybox* s) : 
		skybox(s), sObject(sObject), sSkybox(sSky), sDirShadow(sDirSha), sOmniShadow(sOmniSha),
		mainLight(NULL) {}

	void AddLight(DirectionalLight* l);
	void AddLight(PointLight* l);
	void AddLight(SpotLight* l);

	void AddModel(std::string id, Model m, glm::mat4 modelmat);
	void setModelMatrix(std::string id, glm::mat4 modelmat);
	glm::mat4 getModelMatrix(std::string id);

	// Renders
	void RenderShadowDirLight(DirectionalLight* light);
	void RenderShadowOmniLights(PointLight* light);
	void RenderObjects(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Camera c);
	void RenderScene();

	// Funciones auxiliares
	void SetPointLights(unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(unsigned int textureUnit, unsigned int offset);

	~Renderer();

private:
	// Objectos de renderizacion general
	Skybox* skybox;

	// SOLUCIO TEMPORAL a cargar tots els models amb les seves transformacions mapejats
	std::map<std::string, std::pair<Model, glm::mat4>> Models;

	// Shaders
	Shader* sObject, *sSkybox, *sDirShadow, *sOmniShadow;

	// Lights (SOLUCIO TEMPORAL)
	DirectionalLight* mainLight;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;
	GLuint pointLightCount, spotLightCount;
};