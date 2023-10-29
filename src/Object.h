#pragma once

#ifndef OBJECT_H
#include "stdafx.h"
#include "Material.h"
#include "Mesh.h"

struct Face
{
	unsigned int	iNumVertices;
	unsigned int	iMaterialIndex;
	Vector3D* pVertices;
	Vector3D* pNormals;
	Vector2D* pTexCoords;
};

class Object {
public:
	// Constructors

	_stdcall Object() {}

	// Metodos Publicos de Inicializacion Grafica

	int _stdcall LoadModel(char* szFileName);

	// Metodos Publicos de Limpieza Grafica

	void _stdcall ClearMeshList();

	// Destructors

	virtual _stdcall ~Object();

private:

	// Metodos Privados de Inicializacion

	bool _stdcall LoadMaterialLib(const char szFileName[], unsigned int* iCurMaterialIndex, char szBasePath[]);
	void _stdcall loadToVAOList(const Face* pFaces, const unsigned int iFaceCount);
	void _stdcall GetFaceNormal(float fNormalOut[3], const Face* pFace);
	void _stdcall ParseFaceString(char szFaceString[], Face* FaceOut, const Vector3D* pVertices, const Vector3D* pNormals, const Vector2D* pTexCoords, const unsigned int iMaterialIndex);

	// Atributos Privados Graficos

	std::vector<int> materialIndexes;	// Indices para relacionar materiales con texturas
	std::vector<Mesh> meshList;			// Meshes que componen el objeto
	std::vector<Material> materialList;	// Materiales que componen el objeto

	// Atributos Privados Fisicas


};

// Metodos Adicionales Externos

// Callback function for comparing two faces with qsort
static int CompareFaceByMaterial(const void* Arg1, const void* Arg2);

#endif // !OBJECT_H
