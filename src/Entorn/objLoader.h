//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MONOFINESTRA amb OpenGL 4.3+, interfície GLFW, ImGui i llibreries GLM
//******** Enric Martí (Setembre 2023)
// objLoader.h: Inteface of the class COBJModel.
//
//	  Versió 2.0:	- Adaptació funcions a crear un VAO per a cada material del fitxer
//////////////////////////////////////////////////////////////////////////////////////

#ifndef OBJLOADER_H
#define OBJLOADER_H

#define OBJLOADER_CLASS_DECL     __declspec(dllexport)

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stdafx.h"
#include "../Material.h"

// Màxima mida vector VAOList
#define MAX_SIZE_VAOLIST 125

// Needed structures
struct Vector3D	// Pasada
{
	float fX;
	float fY;
	float fZ;
};

struct Vector2D	// Pasada
{
	float fX;
	float fY;
};

struct OBJFileInfo	// Pasada
{
	unsigned int iVertexCount;
	unsigned int iTexCoordCount;
	unsigned int iNormalCount;
	unsigned int iFaceCount;
	unsigned int iMaterialCount;
};

struct Face	// Pasada
{
	unsigned int	iNumVertices;
	unsigned int	iMaterialIndex;
	Vector3D		*pVertices;
	Vector3D		*pNormals;
	Vector2D		*pTexCoords;
};

class OBJLOADER_CLASS_DECL COBJModel  
{
  public:
	  void _stdcall DrawModel(int prim_Id);		// ?
	  int _stdcall LoadModel(char* szFileName);	// Pasada
	  _stdcall COBJModel();						// Pasada
	  virtual _stdcall ~COBJModel();			// Pasada
	  void _stdcall EliminaLlista(int prim_Id);	// No se llama

	  // Funcions CVAO
	  void _stdcall netejaVAOList_OBJ();		// Pasada
	  void _stdcall netejaTextures_OBJ();		// No se llama
	  void _stdcall draw_TriVAO_OBJ(GLuint sh_programID);	// ?

  private:
	  void _stdcall ReadNextString(char szString[], FILE *hStream);	// Pasada
	  int _stdcall LoadTexture2(const char szFileName[_MAX_PATH]);	// Pasada
	  void _stdcall UseMaterial(Material *pMaterial);				// Pasada
	  void _stdcall UseMaterial_ShaderID(GLuint sh_programID, Material pMaterial);		// Pasada
	  void _stdcall GetTokenParameter(char szString[], const unsigned int iStrSize, FILE *hFile);	// Pasada
	  void _stdcall MakePath(char szFileAndPath[]);		// Pasada
	  bool _stdcall LoadMaterialLib(const char szFileName[], Material *pMaterials,		// Pasada
		  unsigned int *iCurMaterialIndex, char szBasePath[]);
	  CVAO _stdcall RenderToVAOList(const Face* pFaces, const unsigned int iFaceCount,	// ?
		  Material *pMaterials);
	  void _stdcall loadToVAOList(const Face* pFaces, const unsigned int iFaceCount,	// Pasada
		  Material* pMaterials);
	  void _stdcall GetFaceNormal(float fNormalOut[3], const Face *pFace);				// Pasada
	  void _stdcall ParseFaceString(char szFaceString[], Face *FaceOut, const Vector3D *pVertices,	// Pasada
		  const Vector3D *pNormals, const Vector2D *pTexCoords, const unsigned int iMaterialIndex);
	  void _stdcall GetFileInfo(FILE *hStream, OBJFileInfo *Stat, const char szConstBasePath[]);	// Pasada
 	  void _stdcall GenTexCoords();		// Pasada


// CVAO
	  GLint numMaterials = 0;
	  int vector_Materials[MAX_SIZE_VAOLIST];	// Pasada
	  CVAO VAOList_OBJ[MAX_SIZE_VAOLIST];		// Pasada
	  Material vMaterials[MAX_SIZE_VAOLIST];	// Pasada

// Funcions CVAO
	  void _stdcall initVAOList_OBJ();						// Innecesaria
	  void _stdcall Set_VAOList_OBJ(GLint k, CVAO auxVAO);	// Pasada
	  void _stdcall deleteVAOList_OBJ(GLint k);				// Pasada
	  void _stdcall draw_TriVAO_Object_OBJ(GLint k);		// Pasada? (Mesh::RenderMesh())
};


// Callback function for comparing two faces with qsort
static int CompareFaceByMaterial(const void *Arg1, const void *Arg2);	// Pasada

//Returns a newly created COBJModel object. 
OBJLOADER_CLASS_DECL COBJModel* _stdcall InitObject();		// No se llama

//Destroys a COBJModel object
OBJLOADER_CLASS_DECL void _stdcall UnInitObject(COBJModel *obj);	// No se llama

#endif
