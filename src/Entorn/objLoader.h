//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn b�sic VS2022 MONOFINESTRA amb OpenGL 4.3+, interf�cie GLFW, ImGui i llibreries GLM
//******** Enric Mart� (Setembre 2023)
// objLoader.h: Inteface of the class COBJModel.
//
//	  Versi� 2.0:	- Adaptaci� funcions a crear un VAO per a cada material del fitxer
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

// M�xima mida vector VAOList
#define MAX_SIZE_VAOLIST 125

// Needed structures
struct Vector3D
{
	float fX;
	float fY;
	float fZ;
};

struct Vector2D
{
	float fX;
	float fY;
};

struct OBJFileInfo
{
	unsigned int iVertexCount;
	unsigned int iTexCoordCount;
	unsigned int iNormalCount;
	unsigned int iFaceCount;
	unsigned int iMaterialCount;
};

struct Face
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
	  void _stdcall DrawModel(int prim_Id);
	  int _stdcall LoadModel(char* szFileName);
	  _stdcall COBJModel();
	  virtual _stdcall ~COBJModel();
	  void _stdcall EliminaLlista(int prim_Id);

	  // Funcions CVAO
	  void _stdcall netejaVAOList_OBJ();
	  void _stdcall netejaTextures_OBJ();
	  void _stdcall draw_TriVAO_OBJ(GLuint sh_programID);

  private:
	  void _stdcall ReadNextString(char szString[], FILE *hStream);
	  int _stdcall LoadTexture2(const char szFileName[_MAX_PATH]);
	  void _stdcall UseMaterial(Material *pMaterial);
	  void _stdcall UseMaterial_ShaderID(GLuint sh_programID, Material pMaterial);
	  void _stdcall GetTokenParameter(char szString[], const unsigned int iStrSize, FILE *hFile);
	  void _stdcall MakePath(char szFileAndPath[]);
	  bool _stdcall LoadMaterialLib(const char szFileName[], Material *pMaterials,
		  unsigned int *iCurMaterialIndex, char szBasePath[]);
	  CVAO _stdcall RenderToVAOList(const Face* pFaces, const unsigned int iFaceCount,
		  Material *pMaterials);
	  void _stdcall loadToVAOList(const Face* pFaces, const unsigned int iFaceCount,
		  Material* pMaterials);
	  void _stdcall GetFaceNormal(float fNormalOut[3], const Face *pFace);
	  void _stdcall ParseFaceString(char szFaceString[], Face *FaceOut, const Vector3D *pVertices,
		  const Vector3D *pNormals, const Vector2D *pTexCoords, const unsigned int iMaterialIndex);
	  void _stdcall GetFileInfo(FILE *hStream, OBJFileInfo *Stat, const char szConstBasePath[]);
 	  void _stdcall GenTexCoords();


// CVAO
	  GLint numMaterials = 0;
	  int vector_Materials[MAX_SIZE_VAOLIST];
	  CVAO VAOList_OBJ[MAX_SIZE_VAOLIST];
	  Material vMaterials[MAX_SIZE_VAOLIST];

// Funcions CVAO
	  void _stdcall initVAOList_OBJ();
	  void _stdcall Set_VAOList_OBJ(GLint k, CVAO auxVAO);
	  void _stdcall deleteVAOList_OBJ(GLint k);
	  void _stdcall draw_TriVAO_Object_OBJ(GLint k);
};


// Callback function for comparing two faces with qsort
static int CompareFaceByMaterial(const void *Arg1, const void *Arg2);

//Returns a newly created COBJModel object. 
OBJLOADER_CLASS_DECL COBJModel* _stdcall InitObject();

//Destroys a COBJModel object
OBJLOADER_CLASS_DECL void _stdcall UnInitObject(COBJModel *obj);

#endif
