#pragma once

#include "Entorn/stdafx.h"
/*
* Aquesta classe representa una instancia de Llum dins de l'escena
* aquest configurara els parametres de la shader d'il�luminaci� per a que es pugui utilitzar al nostre favor.
*/
class Light
{
public:
	Light();
	Light(bool encesa, float difusa[], float especular[], CEsfe3D posicio, CAtenua atenuacio, bool restringida, float spotdirection[], float spotcoscutoff, float spotexponent);

	void Iluminacio(GLint sh_programID);
	void setShaderLight(int shaderID);

	~Light();
private:
	//TODO: Mirar quins atributs ens interessen tenir a nivell d'instancia

	bool encesa;				// Booleana que controla si la llum �s encesa [TRUE] o no [FALSE]
	CEsfe3D posicio;			// Posici� d ela foont de llum en coordenades esf�riques.
	GLfloat difusa[4];			// Intensitat difusa de la font de llum (r,g,b,a)
	GLfloat especular[4];		// Intensitat especular de la font de llum (r,g,b,a)
	CAtenua atenuacio;			// Coeficients de l'equaci� d'atenuaci� de la llum fatt=1/(ad2+bd+c)
	bool restringida;			// Booleana que indica si la font de llum �s restringida [TRUE] i per tant s�n v�lids els coeficients posteriors o no [FALSE].
	GLfloat spotdirection[3];	// Vector de direci� de la font de llum restringida (x,y,z).
	GLfloat spotcoscutoff;		// Coseno de l'angle d'obertura de la font de llum restringida.
	GLfloat spotexponent;		// Exponent que indica l'atenuaci� de la font del centre de l'eix a l'exterior, segons model de Warn.

	//TODO: mirar si aixo es fa a nivell de classe o a nivell d'instancia
	bool ambient;
	GLfloat ambientg[4];

	bool ifix;
};