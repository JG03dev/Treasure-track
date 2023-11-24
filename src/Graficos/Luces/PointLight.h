#pragma once

#include "Light.h"

#include "../Sombras/OmniShadowMap.h"

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLuint shadowWidth, GLuint shadowHeight,
		GLfloat lnear, GLfloat lfar,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(Shader& s, int nLight);

	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;

	GLfloat farPlane;
};

