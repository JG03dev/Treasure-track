#pragma once

#include "Entorn/stdafx.h"
#include "Shader.h"

class Skybox 
{
public:
	Skybox();
	Skybox(std::vector<std::string> faces, std::string vertexLocation, std::string fragmentLocation);

	void loadCubeSkyboxVAO();
	void loadCubemap(std::vector<std::string> faces);
	void drawSkybox(char eix_Polar, glm::mat4 MatriuProjeccio, glm::mat4 MatriuVista);

	~Skybox();

private:
	CVAO SkyVAO; //VAO del skybox (spoiler es un cub)
	GLuint SkyTexture; //Textura del skybox
	Shader SkyShader;
};