//-----------------------------------------------------------------------------
// Funciones de Renderizacion
//-----------------------------------------------------------------------------

#include "Renderer.h"

Renderer::Renderer(const char* Parser, GLsizei viewPortWidth, GLsizei viewPortHeight) : vwidth(viewPortWidth), vheight(viewPortHeight)
{
	std::ifstream configFile(Parser);
	json config;
	try
	{
		config = json::parse(configFile);
	}
	catch (json::parse_error& ex)
	{
		std::cerr << "JSON Parse error: " << ex.what() << "parse error at byte " << ex.byte << std::endl;
	}

	// Parse shaders
	for (const auto& shader : config["shaders"])
	{
		std::string vert = shader["paths"]["vert"], frag = shader["paths"]["frag"], geom = shader["paths"]["geom"], id = shader["id"];
		shaList[id] = new Shader(vert.c_str(), frag.c_str(), geom.c_str());
	}
	// Accessing models
	for (const auto& model : config["models"]) {
		std::string id = model["id"];
		std::string modelPath = model["path"];
		std::string modelName = model["name"];
		GLint sIntens = model["intensity"], shine = model["shine"];

		Model* m = new Model(sIntens, shine, modelPath, modelName);

		//Model Instancing
		if (model.count("instances")) // Multiple instances
		{
			m->setInstances(model["instances"].size());
			for (const auto& ins : model["instances"])
			{
				glm::mat4 TG(1.0f);
				std::string suffix = ins["suffix"]; //Identificador per la instancia
				// One instance, has geometric transforms
				if (ins.count("transform"))
					TG = parseTransform(ins["transform"]);
				AddModel(id+suffix, m, TG);
			}
			continue;
		}
		glm::mat4 TG(1.0f);
		// One instance, has geometric transforms
		if (model.count("transform"))
			TG = parseTransform(model["transform"]);
		AddModel(id, m, TG);
	}

	/// Accessing lights (first one will be treated as directionals)

	// Directional Light
	for (const auto& DL : config["lights"]["directionalLights"]) {
		GLuint sWidth = DL["shadow"][0], sHeight = DL["shadow"][1]; // Shadow
		GLfloat cR = DL["color"][0], cG = DL["color"][1], cB = DL["color"][2], // Color
			intensity = DL["intensity"], diffuse = DL["diffuse"], // Intensity and Diffuse
			dX = DL["direction"][0], dY = DL["direction"][1], dZ = DL["direction"][2]; // Direction
		bool isOn = false; // initalState
		dirLights.push_back(new DirectionalLight(sWidth, sHeight, cR, cG, cB, intensity, diffuse, dX, dY, dZ, isOn));
	} 
	currentDirLight = 0; 
	dirLights[0]->Toggle();

	// Point Lights
	for (const auto& PL : config["lights"]["pointLights"]) {
		GLuint sWidth = PL["shadow"][0], sHeight = PL["shadow"][1]; // Shadow
		GLfloat lnear = PL["near"], lfar = PL["far"], // near and far
			cR = PL["color"][0], cG = PL["color"][1], cB = PL["color"][2], // Color
			intensity = PL["intensity"], diffuse = PL["diffuse"], // Intensity and Diffuse
			pX = PL["position"][0], pY = PL["position"][1], pZ = PL["position"][2], // Position
			con = PL["CLE"][0], lin = PL["CLE"][1], exp = PL["CLE"][2]; // Constant linear exponent
		bool isOn = PL["initalState"]; // initalState
		pointLights.push_back(new PointLight(sWidth, sHeight, lnear, lfar, cR, cG, cB, intensity, diffuse, pX, pY, pZ, con, lin, exp, isOn));
	}

	for (const auto& SL : config["lights"]["spotLights"]) {
		GLuint sWidth = SL["shadow"][0], sHeight = SL["shadow"][1]; // Shadow
		GLfloat lnear = SL["near"], lfar = SL["far"], // near and far
			cR = SL["color"][0], cG = SL["color"][1], cB = SL["color"][2], // Color
			intensity = SL["intensity"], diffuse = SL["diffuse"], // Intensity and Diffuse
			pX = SL["position"][0], pY = SL["position"][1], pZ = SL["position"][2], // Position
			dX = SL["direction"][0], dY = SL["direction"][1], dZ = SL["direction"][2], // Direction
			con = SL["CLE"][0], lin = SL["CLE"][1], exp = SL["CLE"][2], // Constant linear exponent
			edge = SL["edge"];
		bool isOn = SL["initalState"]; // initalState
		spotLights.push_back(new SpotLight(sWidth, sHeight, lnear, lfar, cR, cG, cB, intensity, diffuse, pX, pY, pZ, dX, dY, dZ, con, lin, exp, edge, isOn));
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
	dirLights.push_back(l);
}

void Renderer::AddLight(PointLight* l)
{
	if (pointLights.size() < MAX_POINT_LIGHTS)
		pointLights.push_back(l);
}

void Renderer::AddLight(SpotLight* l)
{
	if (spotLights.size() < MAX_POINT_LIGHTS)
		spotLights.push_back(l);
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

void Renderer::RenderEverything(Camera c, glm::mat4 projectionMatrix)
{
	// Render Shadows
	RenderShadowDirLight(dirLights[currentDirLight]);
	for (size_t i = 0; i < pointLights.size(); i++)
	{
		RenderShadowOmniLights(pointLights[i]);
	}
	for (size_t i = 0; i < spotLights.size(); i++)
	{
		RenderShadowOmniLights(spotLights[i]);
	}
	// Render Lights
	RenderObjects(c, projectionMatrix);
}

// TODO: check the parameters passed through these functions

void Renderer::RenderShadowDirLight(DirectionalLight* light)
{
	Shader* sDirShadow = shaList["directionalShadow"];
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
	Shader* sOmniShadow = shaList["omniDirectionalShadow"];
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
	Shader* sObject = shaList["objects"];
	sObject->setUniform("pointLightCount", (int)pointLights.size());

	char locBuff[100] = { '\0' };
	for (size_t i = 0; i < pointLights.size(); i++)
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
	Shader* sObject = shaList["objects"];
	sObject->setUniform("spotLightCount", (int)spotLights.size());
	char locBuff[100] = { '\0' };

	for (size_t i = 0; i < spotLights.size(); i++)
	{
		spotLights[i]->UseLight(*sObject, i);

		spotLights[i]->getShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i + offset);
		sObject->setUniform(locBuff, (int)(textureUnit + i));

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i + offset);
		sObject->setUniform(locBuff, spotLights[i]->GetFarPlane());
	}
}

void Renderer::RenderObjects(Camera c, glm::mat4 projectionMatrix)
{
#ifdef __APPLE__
	glViewport(0, 0, 2 * vwidth, 2 * vheight);
#else
	glViewport(0, 0, vwidth, vheight);
#endif
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DirectionalLight* mainLight = dirLights[currentDirLight];
	glm::mat4 viewMatrix = c.getViewMatrix();
	Shader* sObject = shaList["objects"];

	//Skybox
	// TODO: multi skybox drawing??
	skyList[currentSky].Draw(*shaList["skybox"], glm::mat4(glm::mat3(viewMatrix)), projectionMatrix);

	///Objects
	sObject->use();

	//Main matrices
	sObject->setUniform("projection", projectionMatrix);
	sObject->setUniform("view", viewMatrix);
	sObject->setUniform("eyePosition", c.position.x, c.position.y, c.position.z);

	//Lights
	dirLights[currentDirLight]->UseLight(*sObject);

	// TODO: Averiguar que hacen los numeros magicos
	SetPointLights(3, 0);
	SetSpotLights(3 + pointLights.size(), pointLights.size());
	
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
	shaList["objects"]->use();
	for(auto model : Models)
	{
		shaList["objects"]->setUniform("model", model.second.second);
		model.second.first->RenderModel(*shaList["objects"]);
	}
}


Renderer::~Renderer()
{
	///Free all dynamically stored data
	// Maps of pointers
	int nIns = 0;
	for (auto& m : Models)
	{
		if (m.second.first && nIns == 0)
		{
			nIns = m.second.first->getNInstances();
			delete m.second.first;
			m.second.first = nullptr;
		}
		nIns--;
	}
	for (auto s : shaList)
	{
		if (s.second)
		{
			delete s.second;
			s.second = nullptr;
		}
	}
	// Vector of pointers
	for (auto d : dirLights)
	{
		if (d)
		{
			delete d;
			d = nullptr;
		}
	}
	for (auto d : pointLights)
	{
		if (d)
		{
			delete d;
			d = nullptr;
		}
	}
	for (auto d : spotLights)
	{
		if (d)
		{
			delete d;
			d = nullptr;
		}
	}

}

glm::mat4 parseTransform(const json& transformData) {
	if (transformData.empty())
		return glm::mat4(1.0f);

	glm::vec3 translation = {
		transformData["translate"][0],
		transformData["translate"][1],
		transformData["translate"][2]
	};

	glm::vec3 rotation = {
		transformData["rotation"][0],
		transformData["rotation"][1],
		transformData["rotation"][2]
	};

	glm::vec3 scale = {
		transformData["scale"][0],
		transformData["scale"][1],
		transformData["scale"][2]
	};

	glm::mat4 transform = glm::mat4(1.0f);  // Identity matrix
	transform = glm::translate(transform, translation);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, scale);

	return transform;
}