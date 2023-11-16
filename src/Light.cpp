#include "Light.h"


Light::Light()
{
	encesa = false;
	//TODO: Buscar tots els inicialitzadors per defecte

	GLfloat especular[] = { 0.0,0.0,0.0,1.0 };
	GLfloat ambientg[] = { .5,.5,.5,1.0 };

}

Light::Light(bool encesa, float difusa[], float especular[], CEsfe3D posicio, CAtenua atenuacio, bool restringida, float spotdirection[], float spotcoscutoff, float spotexponent)
{
	this->encesa = encesa;
	for (int i = 0; i < 4; i++) {
		this->difusa[i] = difusa[i];
		this->especular[i] = especular[i];
	}
	this->posicio = posicio;
	this->atenuacio = atenuacio;
	this->restringida = restringida;
	for (int i = 0; i < 3; i++) {
		this->spotdirection[i] = spotdirection[i];
	}
	this->spotcoscutoff = spotcoscutoff;
	this->spotexponent = spotexponent;
}

void Light::Iluminacio(GLint sh_programID)
{
	//TODO: Mirar si valdria la pena guardar aquests com a atributs
	GLdouble angv, angh;

	// Configuració per defecte de la posicio
	GLfloat position[] = { 0.0,0.0,200.0,1.0 };


	// Definició de llum ambient segons booleana ll_amb
	if (ambient) glUniform4f(glGetUniformLocation(sh_programID, "LightModelAmbient"), ambientg[0], ambientg[1], ambientg[2], ambientg[3]);
	else glUniform4f(glGetUniformLocation(sh_programID, "LightModelAmbient"), especular[0], especular[1], especular[2], especular[3]);

	// Passar llum fixe a Coordenades Món o llum lligada a càmera
	glUniform1i(glGetUniformLocation(sh_programID, "fixedLight"), ifix);

	// Conversió angles graus -> radians
	angv = posicio.alfa * PI / 180;
	angh = posicio.beta * PI / 180;

	// Conversió Coord. esfèriques -> Coord. cartesianes per a la posició de la llum
	position[0] = posicio.R * cos(angh) * cos(angv);
	position[1] = posicio.R * sin(angh) * cos(angv);
	position[2] = posicio.R * sin(angv);
	position[3] = 1.0;

	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[0].position"), position[0], position[1], position[2], position[3]);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[0].diffuse"), difusa[0], difusa[1], difusa[2], difusa[3]);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[0].specular"), especular[0], especular[1], especular[2], especular[3]);

	// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[0].attenuation"), atenuacio.a, atenuacio.b, atenuacio.c);


	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[0].restricted"), restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[0].spotDirection"), spotdirection[0], spotdirection[1], spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[0].spotCosCutoff"), spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[0].spotExponent"), spotexponent);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[0].sw_light"), encesa);

	//TODO: Portar aquest codi a nivell d'escena
	// Selecció del model d'iluminació segons variable ilumin (i·luminació Suau)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// PRÀCTICA 3: Crida a la funció pel càlcul dels vectors normals normals a les cares si l'objecte és un fractal
	// Il.luminació suau 
	glShadeModel(GL_SMOOTH);

}

Light::~Light()
{
}
