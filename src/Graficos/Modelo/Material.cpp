#include "Material.h"



Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

// TODO: En diciembre las shaders para los materiales seran mas complejos
void Material::UseMaterial(Shader &s)
{
	s.setUniform("material.specularIntensity", specularIntensity);
	s.setUniform("material.shininess", shininess);
}

Material::~Material()
{
}
