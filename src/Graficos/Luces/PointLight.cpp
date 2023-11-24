#include "PointLight.h"



PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
						GLfloat lnear, GLfloat lfar, 
						GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;

	farPlane = lfar;

	float aspect = (float)shadowWidth / (float)shadowHeight;
	lightProj = glm::perspective(glm::radians(90.0f), aspect, lnear, lfar);

	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(Shader& s, int nLight)
{
	char locBuff[100] = { '\0' };

	snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", nLight);
	s.setUniform(locBuff, colour.x, colour.y, colour.z);

	snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", nLight);
	s.setUniform(locBuff, ambientIntensity);

	snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", nLight);
	s.setUniform(locBuff, diffuseIntensity);

	snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", nLight);
	s.setUniform(locBuff, position.x, position.y, position.z);

	snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", nLight);
	s.setUniform(locBuff, constant);

	snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", nLight);
	s.setUniform(locBuff, linear);

	snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", nLight);
	s.setUniform(locBuff, exponent);

}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	//+x, -x
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	//+y, -y
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	//+z, -z
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}

GLfloat PointLight::GetFarPlane()
{
	return farPlane;
}
glm::vec3 PointLight::GetPosition()
{
	return position;
}

PointLight::~PointLight()
{
}
