// material.cpp : Funcions de definició de les propietats de reflectivitat dels objectes.

#include "Entorn/stdafx.h"
#include "Material.h"

void Material::DestroyTextures()
{
	Textura.ClearTexture();
}

Material::~Material()
{
	DestroyTextures();
}

void Material::UseMaterial()
{
////////////////////////////////////////////////////////////////////////
// Make a given material the current one
////////////////////////////////////////////////////////////////////////
float color[4] = { 1.0F, 0.0f, 0.0f, 1.0f };

//glColor3f(1.0,1.0,1.0);
// Look for the presence of a texture and activate texturing if succeed

	if (textureID)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	else {
		glDisable(GL_TEXTURE_2D);
		//glMaterialfv(GL_FRONT, GL_EMISSION, pMaterial->fEmmissive);

		glColorMaterial(GL_FRONT, GL_AMBIENT);
		color[0] = fAmbient[0];	color[1] = fAmbient[1];	color[2] = fAmbient[2];
		//glMaterialfv(GL_FRONT, GL_AMBIENT, pMaterial->fAmbient);
		glMaterialfv(GL_FRONT, GL_AMBIENT, color);
		glColor4fv(color);

		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		color[0] = fDiffuse[0];	color[1] = fDiffuse[1];	color[2] = fDiffuse[2];
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, pMaterial->fDiffuse);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		glColor4fv(color);

		glColorMaterial(GL_FRONT, GL_SPECULAR);
		color[0] = fSpecular[0];	color[1] = fSpecular[1];	color[2] = fSpecular[2];
		//glMaterialfv(GL_FRONT, GL_SPECULAR, pMaterial->fSpecular);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color);
		glColor4fv(color);

		glMaterialf(GL_FRONT, GL_SHININESS, fShininess);

		glEnable(GL_COLOR_MATERIAL);
	}
}

void Material::LoadTexture2(const char szFileName[_MAX_PATH])
{
	////////////////////////////////////////////////////////////////////////
	// Load a texture and return its ID
	////////////////////////////////////////////////////////////////////////

	FILE* file = NULL;
	int errno;
	unsigned int iTexture = 0;


	// Open the image file for reading
	errno = fopen_s(&file, szFileName, "r");			// Funció Visual 2005

	if (errno != 0)
	{
		return;
	}

	// Close the image file
	fclose(file);

	// SOIL_load_OGL_texture: Funció que llegeix la imatge del fitxer filename
	//				si és compatible amb els formats SOIL (BMP,JPG,GIF,TIF,TGA,etc.)
	//				i defineix la imatge com a textura OpenGL retornant l'identificador 
	//				de textura OpenGL.
	iTexture = SOIL_load_OGL_texture
	(szFileName,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y
	);


	// If execution arrives here it means that all went well. Return true

	// Make the texture the current one
	glBindTexture(GL_TEXTURE_2D, iTexture);

	// Build mip-maps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	textureID = iTexture;
}

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
