#pragma once

#include "Mesh.h"
#include <vector>

class Object {
public:

	// Constructors

	_stdcall Object();

	// Netodos publicos

	int _stdcall LoadModel(char* szFileName);

	// Destructors

	virtual _stdcall ~Object();

private:

	// Metodos Privados

	void _stdcall GetFileInfo(FILE* hStream, OBJFileInfo* Stat, const char szConstBasePath[]);

	// Atributos Privados

	Mesh mesh;
	std::vector<Material> vMaterials;

#pragma region DELETE
	void _stdcall ReadNextString(char szString[], FILE* hStream);
	void _stdcall GetTokenParameter(char szString[], const unsigned int iStrSize, FILE* hFile);
	void _stdcall MakePath(char szFileAndPath[]);
	void _stdcall ParseFaceString(char szFaceString[], Face* FaceOut, const Vector3D* pVertices,
		const Vector3D* pNormals, const Vector2D* pTexCoords, const unsigned int iMaterialIndex);
#pragma endregion
};
