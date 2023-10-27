//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn b�sic VS2022 MONOFINESTRA amb OpenGL 4.3+, interf�cie GLFW, ImGui i llibreries GLM
//******** Ferran Poveda, Marc Vivet, Carme Juli�, D�bora Gil, Enric Mart� (Setembre 2023)
// material.h : interface de material.cpp
//

#ifndef GLMATERIAL_H
#define GLMATERIAL_H

#include "stdafx.h"

// TODO: Mirar quins defines ens interessen (per tipus de materials etc.)
#define MAX_MATERIALS  11

//Tipus de materials
#define MAT_CAP 		 -1
#define	MAT_DEFAULT       0
#define MAT_BASE          1
#define MAT_METALL_OBSCUR 2 
#define	MAT_VIDRE		  3
#define MAT_GOMA_NEGRA    4  
#define MAT_GESPA         5
#define MAT_METALL_GROC   6
#define MAT_LLUM_VERMELLA 7
#define MAT_LLUM_GROGA    8
#define MAT_LLUM_VERDA    9
#define MAT_FITXER	     10

// MTL File string indentifiers
#define NEW_MTL_ID		 "newmtl"
#define MTL_TEXTURE_ID   "map_Kd"
#define MTL_AMBIENT_ID	 "Ka"
#define MTL_DIFFUSE_ID	 "Kd"
#define MTL_SPECULAR_ID	 "Ks"
#define MTL_SHININESS_ID "Ns"

//enum 
//{
//	MAT_CAP,
//	MAT_DEFAULT,
//	MAT_BASE, 
//	MAT_METALL_OBSCUR, 
//	MAT_VIDRE, 
//	MAT_GOMA_NEGRA, 
//	MAT_GESPA, 
//	MAT_METALL_GROC,
//	MAT_LLUM_VERMELLA,
//	MAT_LLUM_GROGA,
//	MAT_LLUM_VERDA
//} mat;



/* ------------------------------------------------------------------------- */
/*                            Funcions de material                           */
/* ------------------------------------------------------------------------- */
class Material
{
public:
	Material();

	void UseMaterial();
	void UseMaterial_ShaderID(GLuint sh_programID);
	void LoadTexture2(const char szFileName[_MAX_PATH]);
	void LoadMaterial(FILE* hFileT, char szBasePath[]);
	
	void DestroyTextures();
	~Material();

	char	szName[1024];		 // Name of the material
	GLuint iTextureID;
	char	szTexture[_MAX_PATH];// Filename of the texture
	float fDiffuse[3];		 // Diffuse component
	float fAmbient[3];		 // Ambient component
	float fSpecular[3];		 // Specular component
	float fEmmissive[3];		 // Emmissive component
	float fShininess;			 // Specular exponent
};
#endif