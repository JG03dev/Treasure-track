#pragma once

#include "../../Encabezados/stdafx.h"
#include "../Shader.h"
#include "../Sombras/ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight, 
			GLfloat red, GLfloat green, GLfloat blue,
			GLfloat aIntensity, GLfloat dIntensity);

	ShadowMap* getShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

