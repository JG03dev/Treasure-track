/*
* Classe Renderer: Esta classe se encargara de hacer las funciones de renderizacion de objetos, luces y sombras de la escena,
* Esta evita tener un main o una classe game de 4k lineas por el exceso de funciones que no pertenecen estar en un archivo tan
* general
*/

#include "../Encabezados/stdafx.h"
#include "../Encabezados/json.hpp"

// Renderizacion General
#include "Shader.h"
#include "Skybox.h"
#include "../General/Camera.h"
#include "Modelo/Model.h"

// Iluminado
#include "Luces/DirectionalLight.h"
#include "Luces/PointLight.h"
#include "Luces/SpotLight.h"
#include "Sombras/ShadowMap.h"
#include "Sombras/OmniShadowMap.h"

//Light constants
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;


class Renderer
{
public:
	// Constructors
	Renderer() {}

	Renderer(const char* Parser, GLsizei viewPortWidth, GLsizei viewPortHeight);

	// Data modifiers 

	void AddLight(DirectionalLight* l);
	void AddLight(PointLight* l);
	void AddLight(SpotLight* l);

	void AddModel(std::string id, Model* m, glm::mat4 modelmat);
	void setModelMatrix(std::string id, glm::mat4 modelmat);
	std::pair<Model*, glm::mat4> getModel(std::string id);
	std::map<std::string, std::pair<Model*, glm::mat4>> getModelList() { return Models; }

	void setCurrentSky(int cs) { currentSky = cs; }
	void cycleSky() { currentSky = currentSky == skyList.size() - 1 ? 0 : currentSky + 1; }

	void setCurrentDirLight(int cdl) { currentDirLight = cdl; }
	void cycleDirLight() { currentDirLight = currentDirLight == dirLights.size() - 1 ? 0 : currentDirLight + 1; }

	// Renders

	void RenderEverything(Camera c, glm::mat4 projectionMatrix);
	void RenderShadowDirLight(DirectionalLight* light);
	void RenderShadowOmniLights(PointLight* light);
	void RenderObjects(Camera c, glm::mat4 projectionMatrix);
	void RenderScene();

	// Funciones auxiliares

	void SetPointLights(unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(unsigned int textureUnit, unsigned int offset);

	~Renderer();

private:
	// Skyboxes
	std::vector<Skybox> skyList;
	int currentSky;

	// Modelos
	std::map<std::string, std::pair<Model*, glm::mat4>> Models;

	// Shaders
	std::map<std::string, Shader> shaList;

	// Lights
	std::vector<DirectionalLight*> dirLights; // 0 will be default (sun)
	int currentDirLight;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	//Viewport
	GLsizei vwidth, vheight;
};