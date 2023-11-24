//-----------------------------------------------------------------------------
// Funciones de Renderizacion
//-----------------------------------------------------------------------------

#include "Renderer.h"


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

void Renderer::AddModel(std::string id, Model m, glm::mat4 modelmat)
{	
	Models[id] = std::pair<Model, glm::mat4>(m, modelmat);
}

void Renderer::setModelMatrix(std::string id, glm::mat4 modelmat)
{
	std::map<std::string, std::pair<Model, glm::mat4>>::iterator it = Models.find(id);

	if (it != Models.end())
	{
		// Find it and add it to the map
		Models[id].second = modelmat;
	}
}

glm::mat4 Renderer::getModelMatrix(std::string id)
{

	std::map<std::string, std::pair<Model, glm::mat4>>::iterator it = Models.find(id);

	if (it != Models.end())
	{
		// Find it and add it to the map
		return Models[id].second;
	}
}

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


// This has so many parameters that sould be inside Game / Scene class
void Renderer::RenderObjects(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Camera c)
{
#ifdef __APPLE__
	glViewport(0, 0, 2 * gWindowWidth, 2 * gWindowHeight);
#else
	glViewport(0, 0, gWindowWidth, gWindowHeight);
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Skybox
	skybox->Draw(*sSkybox, glm::mat4(glm::mat3(viewMatrix)), projectionMatrix); //Remove 

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
	for(auto model : Models)
	{
		sObject->setUniform("model", model.second.second);
		model.second.first.Draw(*sObject);
	}
}


Renderer::~Renderer()
{
}