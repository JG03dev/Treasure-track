#include "SpotLight.h"



SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat lnear, GLfloat lfar, 
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat xDir, GLfloat yDir, GLfloat zDir, 
	GLfloat con, GLfloat lin, GLfloat exp, 
	GLfloat edg, bool isOn) : PointLight(shadowWidth, shadowHeight, lnear, lfar, red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp, isOn)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));

	edge = edg;
	procEdge = cosf(glm::radians(edge));
	active = isOn;
}

void SpotLight::UseLight(Shader& s, int nLight)
{
	

	char locBuff[100] = { '\0' };

	//PointLight attributes
	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", nLight);
	s.setUniform(locBuff, colour.x, colour.y, colour.z);	

	if (active)
	{
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", nLight);
		s.setUniform(locBuff, ambientIntensity);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", nLight);
		s.setUniform(locBuff, diffuseIntensity);
	}
	else {
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", nLight);
		s.setUniform(locBuff, 0.0f);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", nLight);
		s.setUniform(locBuff, 0.0f);
	}

	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", nLight);
	s.setUniform(locBuff, position.x, position.y, position.z);

	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", nLight);
	s.setUniform(locBuff, constant);

	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", nLight);
	s.setUniform(locBuff, linear);

	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", nLight);
	s.setUniform(locBuff, exponent);
	
	// SpotLight Attributes
	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", nLight);
	s.setUniform(locBuff, direction.x, direction.y, direction.z);

	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", nLight);
	s.setUniform(locBuff, procEdge);

	snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.active", nLight);
	s.setUniform(locBuff, active);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}
