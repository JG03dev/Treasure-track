#pragma once

#include "../../Encabezados/stdafx.h"

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();

private: 
	GLfloat specularIntensity;
	GLfloat shininess;
};

