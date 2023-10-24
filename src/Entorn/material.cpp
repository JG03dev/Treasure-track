// material.cpp : Funcions de definició de les propietats de reflectivitat dels objectes.

#include "stdafx.h"
#include "material.h"

// Tipus de materials que es seleccionen a la funció SeleccionaMaterial
MATERIAL  materials[MAX_MATERIALS] =
{
	{//Default
		{0.2f, 0.2f, 0.2f, 1.0f},	// Ambient
		{0.8f, 0.8f, 0.8f, 1.0f},	// Difusse
		{0.0f, 0.0f, 0.0f, 1.0f},	// Specular
		{0.0f, 0.0f, 0.0f, 1.0f},	// Emission
		0							// Shinniness
	},
	{//Material base
		{0, 0, 0.75f, 1},
		{0, 0, 0.75f, 1},
		{0.0f, 0.5f, 1, 1},
		{0, 0, 0, 1},
		20
	},

	{//Metall Obscur
		{0, 0, 0, 1},
		{0.5f, 0.5f, 0.5f, 1},
		{0.7f, 0.7f, 0.7f, 1},
		{0, 0, 0, 1},
		100
	},

	{//Vidre
		{0.2f, 0.65f, 0.9f, 0.5},
		{0.3f, 0.75f, 1, 0.5},
		{1, 1, 1, 1},
		{0, 0, 0, 1},
		50
	},

	{//Goma Negra
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0.3f, 0.3f, 0.3f, 1},
		{0, 0, 0, 1},
		20
	},

	{//Gespa
		{0, 0.2f, 0, 1},
		{0.1f, 0.5f, 0.2f, 1},
		{0.1f, 0.5f, 0.2f, 1},
		{0, 0, 0, 1},
		0
	},

	{//Metall Groc
		{0.3f, 0.3f, 0.1f, 1},
		{1, 1, 0.25f, 1},
		{1, 1, 0.5f, 1},
		{0, 0, 0, 1},
		128
	},

	{//Llum vermella
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0.5f, 0.5f, 1},
		{1, 0, 0, 1},
		128
	},

	{//Llum Groga
		{1.0f, 1.0f, 0, 1},
		{1, 1.0f, 0, 1},
		{1, 1.0f, 0, 1},
		{1, 0.8f, 0, 1},
		128
	},

	{//Llum verda
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0.5f, 1, 0.5f, 1},
		{0, 1, 0, 1},
		128
	},

	{// Fitxer
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		0
	}

};



// llegir_Material: Llegir propietats de material de d'un fitxer segons l'estructura MATERIAL
//			Retorna l'estructura MATERIAL llegida i un booleà si s'ha trobat el fitxer [TRUE] o no [FALSE].
bool llegir_Material(GLuint sh_programID, char* filename,int index)
{ 
	FILE *fd;
	char strMessage[255] = { 0 };
	GLfloat auxemis[4],auxamb[4],auxdiff[4],auxspec[4];
	GLfloat auxshin;

// Inicialitzar variable
	auxamb[0]=0.0f;			auxamb[1]=0.0f;			auxamb[2]=0.0f;			auxamb[3]=1.0f;
	auxemis[0]=0.0f;		auxemis[1]=0.0f;		auxemis[2]=0.0f;		auxemis[3]=1.0f;
	auxdiff[0]=0.0f,		auxdiff[1]=0.0f;		auxdiff[2]=0.0f;		auxdiff[3]=1.0f;
	auxspec[0]=0.0f;		auxspec[1]=0.0f;		auxspec[2]=0.0f;		auxspec[3]=1.0f;
	auxshin=0.0f;

	if ( (fopen_s(&fd,filename,"rt"))!=0)
		{	fprintf(stderr, "ERROR: Unable to find the file: %s!", filename);
			return false;
		}

	fscanf(fd,"%f %f %f %f \n",&auxemis[0],&auxemis[1],&auxemis[2],&auxemis[3]);
	fscanf(fd,"%f %f %f %f \n",&auxamb[0],&auxamb[1],&auxamb[2],&auxamb[3]);
	fscanf(fd,"%f %f %f %f \n",&auxdiff[0],&auxdiff[1],&auxdiff[2],&auxdiff[3]);
	fscanf(fd,"%f %f %f %f \n",&auxspec[0],&auxspec[1],&auxspec[2],&auxspec[3]);
	fscanf(fd,"%f \n",&auxshin);

	fclose(fd);

	materials[index].emission[0]=auxemis[0];	materials[index].emission[1]=auxemis[1];
	materials[index].emission[2]=auxemis[2];	materials[index].emission[3]=auxemis[3];
// Pas component emission del material al Shader
	glUniform4f(glGetUniformLocation(sh_programID, "material.emission"), materials[index].emission[0], materials[index].emission[1], materials[index].emission[2], materials[index].emission[3]);

	materials[index].ambient[0]=auxamb[0];		materials[index].ambient[1]=auxamb[1];
	materials[index].ambient[2]=auxamb[2];		materials[index].ambient[3]=auxamb[3];
// Pas component ambient del material al Shader
	glUniform4f(glGetUniformLocation(sh_programID, "material.ambient"), materials[index].ambient[0], materials[index].ambient[1], materials[index].ambient[2], materials[index].ambient[3]);
	
	materials[index].diffuse[0]=auxdiff[0];		materials[index].diffuse[1]=auxdiff[1];
	materials[index].diffuse[2]=auxdiff[2];		materials[index].diffuse[3]=auxdiff[3];
	// Pas component diffuse del material al Shader
	glUniform4f(glGetUniformLocation(sh_programID, "material.diffuse"), materials[index].diffuse[0], materials[index].diffuse[1], materials[index].diffuse[2], materials[index].diffuse[3]);

	materials[index].specular[0]=auxspec[0];	materials[index].specular[1]=auxspec[1];
	materials[index].specular[2]=auxspec[2];	materials[index].specular[3]=auxspec[3];
	materials[index].shininess=auxshin;
// Pas component specular i shininess del material al Shader
	glUniform4f(glGetUniformLocation(sh_programID, "material.specular"), materials[index].specular[0], materials[index].specular[1], materials[index].specular[2], materials[index].specular[3]);
	glUniform1f(glGetUniformLocation(sh_programID, "material.shininess"), materials[index].shininess);

	return true;
}
