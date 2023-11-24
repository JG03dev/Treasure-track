#pragma once

#include "../../Encabezados/stdafx.h"
#include "../Shader.h"

/*
* Ejemplos de materiales:			Intensity	Shine
* Material poco brillante (dull):	0.3f,		4
* Material mas brillante (shiny):	4.0f,		256
* 
* Merge-note: de momento los materiales seran customs (como los de arriba) hasta que implementemos las
* shaders que admiten el formato para los materiales cargados en los modelos (como el que hay en Modelo-diciembre)
* para la version 1 de momento tiraremos de estos basicos para los objetos del mapa.
*/

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	Material(Material& m) : specularIntensity(m.getSpecIntensity()), shininess(m.getShininess()) {}

	void UseMaterial(Shader& s);

	GLfloat getSpecIntensity() { return specularIntensity; }
	GLfloat getShininess() { return shininess; }

	~Material();

private: 
	GLfloat specularIntensity;
	GLfloat shininess;
};

