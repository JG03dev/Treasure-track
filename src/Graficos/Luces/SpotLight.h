#pragma once

#include "PointLight.h"

class SpotLight :
	public PointLight
{
public:
	SpotLight();

	SpotLight(GLuint shadowWidth, GLuint shadowHeight,
		GLfloat lnear, GLfloat lfar, 
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);

	void UseLight(Shader& s, int nLight);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);

	void Toggle() { isOn = !isOn; }

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;

	bool isOn;
};

