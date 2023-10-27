// material.cpp : Funcions de definició de les propietats de reflectivitat dels objectes.

#include "Material.h"

Material::Material()
{
	iTextureID = 0;
	fEmmissive[0] = 0.0f;	fEmmissive[1] = 0.0f;	fEmmissive[2] = 0.0f;
	fAmbient[0] = 0.0f;		fAmbient[1] = 0.0f;		fAmbient[2] = 0.0f;
	fDiffuse[0] = 0.0f;		fDiffuse[1] = 0.0f;		fDiffuse[2] = 0.0f;
	fAmbient[0] = 0.0f;		fAmbient[1] = 0.0f;		fAmbient[2] = 0.0f;
	fSpecular[0] = 0.0f;		fSpecular[1] = 0.0f;		fSpecular[2] = 0.0f;
	fShininess = 0.0f;
}

void Material::UseMaterial()
{
////////////////////////////////////////////////////////////////////////
// Make a given material the current one
////////////////////////////////////////////////////////////////////////
float color[4] = { 1.0F, 0.0f, 0.0f, 1.0f };

// Look for the presence of a texture and activate texturing if succeed

	if (iTextureID)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, iTextureID);
	}
	else {
		glDisable(GL_TEXTURE_2D);

		glColorMaterial(GL_FRONT, GL_AMBIENT);
		color[0] = fAmbient[0];	color[1] = fAmbient[1];	color[2] = fAmbient[2];
		glMaterialfv(GL_FRONT, GL_AMBIENT, color);
		glColor4fv(color);

		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		color[0] = fDiffuse[0];	color[1] = fDiffuse[1];	color[2] = fDiffuse[2];
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		glColor4fv(color);

		glColorMaterial(GL_FRONT, GL_SPECULAR);
		color[0] = fSpecular[0];	color[1] = fSpecular[1];	color[2] = fSpecular[2];
		glMaterialfv(GL_FRONT, GL_SPECULAR, color);
		glColor4fv(color);

		glMaterialf(GL_FRONT, GL_SHININESS, fShininess);

		glEnable(GL_COLOR_MATERIAL);
	}
}

void Material::UseMaterial_ShaderID(GLuint sh_programID)
{
	////////////////////////////////////////////////////////////////////////
// Make a given material the current one
////////////////////////////////////////////////////////////////////////
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Look for the presence of a texture and activate texturing if succeed
	if (szName != "")
	{
		if (iTextureID)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, iTextureID);
			glUniform1i(glGetUniformLocation(sh_programID, "texture0"), GLint(0));

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		}
		else {
			glDisable(GL_TEXTURE_2D);

			color[0] = fAmbient[0];	color[1] = fAmbient[1];	color[2] = fAmbient[2];

			glUniform4f(glGetUniformLocation(sh_programID, "material.ambient"), color[0], color[1], color[2], color[3]);

			color[0] = fDiffuse[0];	color[1] = fDiffuse[1];	color[2] = fDiffuse[2];

			glUniform4f(glGetUniformLocation(sh_programID, "material.diffuse"), color[0], color[1], color[2], color[3]);

			color[0] = fSpecular[0];	color[1] = fSpecular[1];	color[2] = fSpecular[2];

			glUniform4f(glGetUniformLocation(sh_programID, "material.specular"), color[0], color[1], color[2], color[3]);

			glUniform1f(glGetUniformLocation(sh_programID, "material.shininess"), fShininess);

			glEnable(GL_COLOR_MATERIAL);
		}
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

	iTextureID = iTexture;
}

void Material::LoadMaterial(FILE* hFileT, char szBasePath[])
{
////////////////////////////////////////////////////////////////////////
// Loads a material library file (.mtl)
////////////////////////////////////////////////////////////////////////

	char szString[MAX_STR_SIZE];	// Buffer used while reading the file
// Quit reading when end of file has been reached
	while (!feof(hFileT))
	{
		// Get next string
		ReadNextString(szString, hFileT);

		// Ambient material properties
		if (!strncmp(szString, MTL_AMBIENT_ID, sizeof(MTL_AMBIENT_ID)))
		{
			// Read into current material
			fscanf(hFileT, "%f %f %f",
				&fAmbient[0],
				&fAmbient[1],
				&fAmbient[2]);
		}
// Diffuse material properties
		else if (!strncmp(szString, MTL_DIFFUSE_ID, sizeof(MTL_DIFFUSE_ID)))
		{
			// Read into current material
			fscanf(hFileT, "%f %f %f",
				&fDiffuse[0],
				&fDiffuse[1],
				&fDiffuse[2]);
		}

// Specular material properties
		else if (!strncmp(szString, MTL_SPECULAR_ID, sizeof(MTL_SPECULAR_ID)))
		{
			// Read into current material
			fscanf(hFileT, "%f %f %f",
				&fSpecular[0],
				&fSpecular[1],
				&fSpecular[2]);
		}

// Texture map name
		else if (!strncmp(szString, MTL_TEXTURE_ID, sizeof(MTL_TEXTURE_ID)))
		{
			// Read texture filename
			GetTokenParameter(szString, sizeof(szString), hFileT);
			// Append material library filename to the model's base path
			char szTextureFile[_MAX_PATH];
			strcpy_s(szTextureFile, szBasePath);
			strcat_s(szTextureFile, szString);
			// Store texture filename in the structure
			strcpy_s(szTexture, szTextureFile);
			// Load texture and store its ID in the structure
			LoadTexture2(szTextureFile);
		}

// Shininess
		else if (!strncmp(szString, MTL_SHININESS_ID, sizeof(MTL_SHININESS_ID)))
		{
			// Read into current material
			fscanf(hFileT, "%f",
				&fShininess);
			// OBJ files use a shininess from 0 to 1000; Scale for OpenGL
			fShininess /= 1000.0f;
			fShininess *= 128.0f;
		}
		else //TODO mirar si realment s'acaba un cop no es pot parsejar res mes
			break;
	}
}

void Material::DestroyTextures()
{
	//TODO: Mirar que se tiene que destruir
}

Material::~Material()
{
	DestroyTextures();
	//TODO: Identificar memory leaks
}

