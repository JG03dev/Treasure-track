//-----------------------------------------------------------------------------
// Funciones de Renderizacion
//-----------------------------------------------------------------------------

#include "Renderer.h"


Renderer::Renderer(	const char* shaderObjvert, const char* shderObjfrag, const char* shderObjgeom, 
					const char* shaderSkyvert, const char* shaderSkyfrag, const char* shaderSkygeom,
					const char* shaderDirShavert, const char* shaderDirShafrag, const char* shaderDirShageom,
					const char* shaderOmniShavert, const char* shaderOmniShafrag, const char* shaderOmniShageom,
					Skybox* s, GLsizei viewPortWidth, GLsizei viewPortHeight)
	:
	skybox(s), mainLight(NULL), pointLightCount(0), spotLightCount(0), vwidth(viewPortWidth), vheight(viewPortHeight)
{
	sObject = new Shader(shaderObjvert, shderObjfrag, shderObjgeom);
	sSkybox = new Shader(shaderSkyvert, shaderSkyfrag, shaderSkygeom);
	sDirShadow = new Shader(shaderDirShavert, shaderDirShafrag, shaderDirShageom);
	sOmniShadow = new Shader(shaderOmniShavert, shaderOmniShafrag, shaderOmniShageom);
}

Renderer::Renderer(const char* Parser, GLsizei viewPortWidth, GLsizei viewPortHeight) : vwidth(viewPortWidth), vheight(viewPortHeight)
{

	std::ifstream configFile(Parser);
	nlohmann::json config;
	configFile >> config;

	// Parse shaders
	for (const auto& shader : config["shaders"])
	{
		std::string vert = shader["vert"], frag = shader["frag"], geom = shader["geom"], id = shader["id"];
		shaList[id] = Shader(vert.c_str(), frag.c_str(), geom.c_str());
	}
	// Accessing models
	for (const auto& model : config["models"]) {
		std::string id = model["id"];
		std::string modelPath = model["path"];
		std::string modelName = model["name"];
		GLint sIntens = model["intensity"], shine = model["shine"];

		glm::mat4 transformationMatrix(1.0f);
		//TODO: make a function where it parses and applies the geometric transforms

		AddModel(id, new Model(sIntens, shine, modelPath, modelName), transformationMatrix);
	}

	// Accessing lights (first one will be treated as directionals)
	dirLightCount = config["lights"]["directionalLights"].size();
	for (const auto& DL : config["lights"]["directionalLights"]) {
		GLuint sWidth = DL["shadow"][0], sHeight = DL["shadow"][1]; // Shadow
		GLfloat cR = DL["color"][0], cG = DL["color"][1], cB = DL["color"][2], // Color
			intensity = DL["intensity"], diffuse = DL["diffuse"], // Intensity and Diffuse
			dX = DL["direction"][0], dY = DL["direction"][1], dZ = DL["direction"][2]; // Direction
		bool isOn = false; // initalState
		lightList.push_back(new DirectionalLight(sWidth, sHeight, cR, cG, cB, intensity, diffuse, dX, dY, dZ, isOn));
	} currentDirLight = 0; lightList[0]->Toggle();
	pointLightCount = config["lights"]["pointLights"].size();
	for (const auto& PL : config["lights"]["pointLights"]) {
		GLuint sWidth = PL["shadow"][0], sHeight = PL["shadow"][1]; // Shadow
		GLfloat lnear = PL["near"], lfar = PL["far"], // near and far
			cR = PL["color"][0], cG = PL["color"][1], cB = PL["color"][2], // Color
			intensity = PL["intensity"], diffuse = PL["diffuse"], // Intensity and Diffuse
			pX = PL["position"][0], pY = PL["position"][1], pZ = PL["position"][2], // Position
			con = PL["CLE"][0], lin = PL["CLE"][1], exp = PL["CLE"][2]; // Constant linear exponent
		bool isOn = PL["initialState"]; // initalState
		lightList.push_back(new PointLight(sWidth, sHeight, lnear, lfar, cR, cG, cB, intensity, diffuse, pX, pY, pZ, con, lin, exp, isOn));
	}
	spotLightCount = config["lights"]["spotLights"].size();
	for (const auto& SL : config["lights"]["spotLights"]) {
		GLuint sWidth = SL["shadow"][0], sHeight = SL["shadow"][1]; // Shadow
		GLfloat lnear = SL["near"], lfar = SL["far"], // near and far
			cR = SL["color"][0], cG = SL["color"][1], cB = SL["color"][2], // Color
			intensity = SL["intensity"], diffuse = SL["diffuse"], // Intensity and Diffuse
			pX = SL["position"][0], pY = SL["position"][1], pZ = SL["position"][2], // Position
			dX = SL["direction"][0], dY = SL["direction"][1], dZ = SL["direction"][2], // Direction
			con = SL["CLE"][0], lin = SL["CLE"][1], exp = SL["CLE"][2], // Constant linear exponent
			edge = SL["edge"];
		bool isOn = SL["initialState"]; // initalState
		lightList.push_back(new SpotLight(sWidth, sHeight, lnear, lfar, cR, cG, cB, intensity, diffuse, pX, pY, pZ, dX, dY, dZ, con, lin, exp, edge, isOn));
	}

	// Accessing skyboxes
	for (const auto& skybox : config["skyboxes"]) {
		// Mapping should match with the skybox
		std::vector<std::string> faces = {
		skybox["faces"][0],			// right
		skybox["faces"][1],			// left
		skybox["faces"][2],			// top
		skybox["faces"][3],			// bottom
		skybox["faces"][4],			// front
		skybox["faces"][5]			// back
		};
		skyList.push_back(Skybox(faces));
	}
	// First skybox is current by default
	currentSky = 0;
}

void Renderer::AddLight(DirectionalLight* l)
{
	mainLight = l;
}

void Renderer::AddLight(PointLight* l)
{
	if (pointLightCount < MAX_POINT_LIGHTS)
	{
		pointLights.push_back(l);
		pointLightCount++;
	}
}

void Renderer::AddLight(SpotLight* l)
{
	if (spotLightCount < MAX_POINT_LIGHTS)
	{
		spotLights.push_back(l);
		spotLightCount++;
	}
}

void Renderer::AddModel(std::string id, Model* m, glm::mat4 modelmat)
{	
	Models[id] = std::pair<Model*, glm::mat4>(m, modelmat);
}

void Renderer::setModelMatrix(std::string id, glm::mat4 modelmat)
{
	std::map<std::string, std::pair<Model*, glm::mat4>>::iterator it = Models.find(id);

	if (it != Models.end())
	{
		// Find it and add it to the map
		Models[id].second = modelmat;
	}
}

std::pair <Model*, glm::mat4> Renderer::getModel(std::string id)
{

	std::map<std::string, std::pair<Model*, glm::mat4>>::iterator it = Models.find(id);

	if (it != Models.end())
	{
		// Find it and add it to the map
		return Models[id];
	}
	return std::pair<Model*, glm::mat4>(nullptr, glm::mat4(1.0f));
}

void Renderer::RenderEverything(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Camera c)
{
	// Render Shadows
	RenderShadowDirLight(mainLight);
	for (size_t i = 0; i < pointLightCount; i++)
	{
		RenderShadowOmniLights(pointLights[i]);
	}
	for (size_t i = 0; i < spotLightCount; i++)
	{
		RenderShadowOmniLights(spotLights[i]);
	}
	// Render Lights
	RenderObjects(viewMatrix, projectionMatrix, c);
}

// TODO: check the parameters passed through these functions

void Renderer::RenderShadowDirLight(DirectionalLight* light)
{
	sDirShadow->use();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	sDirShadow->setUniform("directionalLightTransform", &light->CalculateLightTransform());

	sDirShadow->Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderShadowOmniLights(PointLight* light)
{
	sOmniShadow->use();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	sOmniShadow->setUniform("lightPos", light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	sOmniShadow->setUniform("farPlane", light->GetFarPlane());

	// SetLight Matrices
	std::vector<glm::mat4> lightMatrices = light->CalculateLightTransform();
	for (size_t i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		sOmniShadow->setUniform(locBuff, lightMatrices[i]);
	}

	sOmniShadow->Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::SetPointLights(unsigned int textureUnit, unsigned int offset)
{
	if (pointLightCount > MAX_POINT_LIGHTS) pointLightCount = MAX_POINT_LIGHTS;

	sObject->setUniform("pointLightCount", (int)pointLightCount);

	char locBuff[100] = { '\0' };
	for (size_t i = 0; i < pointLightCount; i++)
	{
		pointLights[i]->UseLight(*sObject, i);

		pointLights[i]->getShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		
		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i + offset);
		sObject->setUniform(locBuff, (int) (textureUnit + i));

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i + offset);
		sObject->setUniform(locBuff, pointLights[i]->GetFarPlane());
	}
}

void Renderer::SetSpotLights(unsigned int textureUnit, unsigned int offset)
{
	if (spotLightCount > MAX_SPOT_LIGHTS) spotLightCount = MAX_SPOT_LIGHTS;

	sObject->setUniform("spotLightCount", (int)spotLightCount);
	char locBuff[100] = { '\0' };

	for (size_t i = 0; i < spotLightCount; i++)
	{
		spotLights[i]->UseLight(*sObject, i);

		spotLights[i]->getShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i + offset);
		sObject->setUniform(locBuff, (int)(textureUnit + i));

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i + offset);
		sObject->setUniform(locBuff, spotLights[i]->GetFarPlane());
	}
}

void Renderer::RenderObjects(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Camera c)
{
#ifdef __APPLE__
	glViewport(0, 0, 2 * vwidth, 2 * vheight);
#else
	glViewport(0, 0, vwidth, vheight);
#endif
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Skybox
	skybox->Draw(*sSkybox, glm::mat4(glm::mat3(viewMatrix)), projectionMatrix); //TODO: Check if we can save parameters

	///Objects
	sObject->use();

	//Main matrices
	sObject->setUniform("projection", projectionMatrix);
	sObject->setUniform("view", viewMatrix);
	sObject->setUniform("eyePosition", c.position.x, c.position.y, c.position.z);

	//Lights
	mainLight->UseLight(*sObject);

	// TODO: Averiguar que hacen los numeros majicos
	SetPointLights(3, 0);
	SetSpotLights(3 + pointLightCount, pointLightCount);

	sObject->setUniform("uniformDirectionalLightTransform", mainLight->CalculateLightTransform());

	//Light Shadow Map
	mainLight->getShadowMap()->Read(GL_TEXTURE2);
	sObject->setUniform("theTexture", 1);
	sObject->setUniform("directionalShadowMap", 2);

	sObject->Validate();

	RenderScene();
}

void Renderer::RenderScene()
{
	sObject->use();
	for(auto model : Models)
	{
		sObject->setUniform("model", model.second.second);
		model.second.first->RenderModel(*sObject);
	}
}


Renderer::~Renderer()
{
	for (int i = 0; i < lightList.size(); i++)
	{
		delete lightList[i];
	}
}