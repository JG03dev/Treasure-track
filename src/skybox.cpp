#include "stdafx.h"
#include "skybox.h"

Skybox::Skybox()
{
	loadCubeSkyboxVAO();
	SkyTexture = 0;
}

Skybox::Skybox(std::vector<std::string> faces, std::string vertexLocation, std::string fragmentLocation)
{
	loadCubeSkyboxVAO();
	SkyTexture = 0;
	loadCubemap(faces);
	SkyShader.CreateFromFiles(vertexLocation, fragmentLocation);
}

void Skybox::loadCubeSkyboxVAO()
{
	GLfloat skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	SkyVAO.vaoId = 0;	SkyVAO.vboId = 0;	SkyVAO.eboId = 0;	SkyVAO.nVertexs = 0; SkyVAO.nIndices = 0;

	glGenVertexArrays(1, &SkyVAO.vaoId); //glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &SkyVAO.vboId); //glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(SkyVAO.vaoId); // glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, SkyVAO.vboId); // glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	SkyVAO.nVertexs = 36;
}

void Skybox::loadCubemap(std::vector<std::string> faces)
{
	GLuint auxTexture = SkyTexture; //Auxiliar texture in case something fails

	glGenTextures(1, &SkyTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyTexture);

	int width, height; // nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			SOIL_free_image_data(data);
		}
		else
		{	//std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			fprintf(stderr, "Cubemap texture failed to load at path: %s \n", faces[i].c_str());
			SOIL_free_image_data(data);
			SkyTexture = auxTexture;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::drawSkybox(char eix_Polar, glm::mat4 MatriuProjeccio, glm::mat4 MatriuVista)
{
	GLuint sk_programID = SkyShader.GetShaderID();
	glm::mat4 ModelMatrix(1.0);

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	// Activació shader per a cub skybox
	glUseProgram(sk_programID);

	// Pas Matrius Projecció i Vista a shader
	glUniformMatrix4fv(glGetUniformLocation(sk_programID, "projectionMatrix"), 1, GL_FALSE, &MatriuProjeccio[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(sk_programID, "viewMatrix"), 1, GL_FALSE, &MatriuVista[0][0]);

	// Rotar skyBox per a orientar sobre eix superior Z o X en Vista Esfèrica (POLARX, POLARY, POLARZ)
	if (eix_Polar == POLARZ) ModelMatrix = glm::rotate(ModelMatrix, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	else if (eix_Polar == POLARX) ModelMatrix = glm::rotate(ModelMatrix, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));

	// Escalar Cub Skybox a 5000 per encabir objectes escena a l'interior
	ModelMatrix = glm::scale(ModelMatrix, vec3(5000.0f, 5000.0f, 5000.0f));		//glScaled(5000.0, 5000.0, 5000.0);
	glUniformMatrix4fv(glGetUniformLocation(sk_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);

	// Activar textura cubemaps del Skybox per encabir objectes escena
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyTexture);

	// Attribute Locations must be setup before calling glLinkProgram()
	glBindAttribLocation(sk_programID, 0, "in_Vertex"); // Vèrtexs

	//  Dibuix del Skybox
	glBindVertexArray(SkyVAO.vaoId);
	glDrawArrays(GL_TRIANGLES, 0, SkyVAO.nVertexs);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS); // set depth function back to default
}

Skybox::~Skybox()
{
	//TODO clear VAO
}
