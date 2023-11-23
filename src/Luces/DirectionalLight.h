#pragma once
#include "Light.h"
#include "../General/Shader.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(Shader s);

	~DirectionalLight();

private:
	glm::vec3 direction;
};
