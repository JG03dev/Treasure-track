#include "Object.h"

// OBJ File string indentifiers
#define VERTEX_ID		 "v"
#define TEXCOORD_ID		 "vt"
#define NORMAL_ID		 "vn"
#define FACE_ID			 "f"
#define COMMENT_ID		 "#"
#define MTL_LIB_ID		 "mtllib"
#define USE_MTL_ID		 "usemtl"

// Maximum number of vertices a that a single face can have
#define MAX_VERTICES 256

// Metodos Publicos de Inicializacion Grafica

int _stdcall Object::LoadModel(char* szFileName)
{
	////////////////////////////////////////////////////////////////////////
	// Load a OBJ file and render its data into a display list
	////////////////////////////////////////////////////////////////////////
	OBJFileInfo OBJInfo;		  // General informations about the model
	OBJFileInfo CurrentIndex;	  // Current array index
	char szString[MAX_STR_SIZE];  // Buffer string for reading the file 
	char szBasePath[_MAX_PATH];	  // Path were all paths in the OBJ start
	int nScanReturn = 0;		  // Return value of fscanf
	float fCoord = 0;			  // Buffer float for reading the file
	int i;						  // Loop variable
	unsigned int iCurMaterial = 0;// Current material

	FILE* hFile = NULL;

	// Get base path
	strcpy(szBasePath, szFileName);
	MakePath(szBasePath);

	////////////////////////////////////////////////////////////////////////
	// Open the OBJ file
	////////////////////////////////////////////////////////////////////////
	errno = 0;
	errno = fopen_s(&hFile, szFileName, "r");

	// Success ?
	if (errno != 0)
		return errno;

	////////////////////////////////////////////////////////////////////////
	// Allocate space for structures that hold the model data
	////////////////////////////////////////////////////////////////////////

	// Which data types are stored in the file ? How many of each type ?
	GetFileInfo(hFile, &OBJInfo, szBasePath);

	// Vertices and face
	Vector3D* pVertices = new Vector3D[OBJInfo.iVertexCount];
	Face* pFaces = new Face[OBJInfo.iFaceCount];

	// Allocate space for optional model data only if present.
	Vector3D* pNormals = 0;
	Vector2D* pTexCoords = 0;
	if (OBJInfo.iNormalCount)
		pNormals = new Vector3D[ObjInfo.iNormalCount];
	if (OBJInfo.iTexCoordCount)
		pTexCoords = new Vector2D[OBJInfo.iTexCoordCount];
	
	// Init structure that holds the current array index
	memset(&CurrentIndex, 0, sizeof(OBJFileInfo));

	////////////////////////////////////////////////////////////////////////
	// Read the file contents
	////////////////////////////////////////////////////////////////////////

	// Start reading the file from the start
	rewind(hFile);

	// Quit reading when end of file has been reached
	while (!feof(hFile))
	{
		// Get next string
		ReadNextString(szString, hFile);

		// Next three elements are floats of a vertex
		if (!strncmp(szString, VERTEX_ID, sizeof(VERTEX_ID)))
		{
			// Read three floats out of the file
			nScanReturn = fscanf_s(hFile, "%f %f %f",
				&pVertices[CurrentIndex.iVertexCount].x,
				&pVertices[CurrentIndex.iVertexCount].fY,
				&pVertices[CurrentIndex.iVertexCount].fZ);
			// Next vertex
			CurrentIndex.iVertexCount++;
		}

		// Next two elements are floats of a texture coordinate
		if (!strncmp(szString, TEXCOORD_ID, sizeof(TEXCOORD_ID)))
		{
			// Read two floats out of the file
			nScanReturn = fscanf_s(hFile, "%f %f",
				&pTexCoords[CurrentIndex.iTexCoordCount].fX,
				&pTexCoords[CurrentIndex.iTexCoordCount].fY);
			// Next texture coordinate
			CurrentIndex.iTexCoordCount++;
		}

		// Next three elements are floats of a vertex normal
		if (!strncmp(szString, NORMAL_ID, sizeof(NORMAL_ID)))
		{
			// Read three floats out of the file
			nScanReturn = fscanf_s(hFile, "%f %f %f",
				&pNormals[CurrentIndex.iNormalCount].fX,
				&pNormals[CurrentIndex.iNormalCount].fY,
				&pNormals[CurrentIndex.iNormalCount].fZ);
			// Next normal
			CurrentIndex.iNormalCount++;
		}

		// Rest of the line contains face information
		if (!strncmp(szString, FACE_ID, sizeof(FACE_ID)))
		{
			// Read the rest of the line (the complete face)
			GetTokenParameter(szString, sizeof(szString), hFile);
			// Convert string into a face structure
			ParseFaceString(szString, &pFaces[CurrentIndex.iFaceCount],
				pVertices, pNormals, pTexCoords, iCurMaterial);
			// Next face
			CurrentIndex.iFaceCount++;
		}

		// Process material information only if needed
		if (OBJInfo.iMaterialCount)
		{
			// Rest of the line contains the name of a material
			if (!strncmp(szString, USE_MTL_ID, sizeof(USE_MTL_ID)))
			{
				// Read the rest of the line (the complete material name)
				GetTokenParameter(szString, sizeof(szString), hFile);
				// Are any materials loaded ?
				if (pMaterials)
					// Find material array index for the material name
					for (i = 0; i < (int)OBJInfo.iMaterialCount; i++)
						if (!strncmp(pMaterials[i].szName, szString, sizeof(szString)))
						{
							iCurMaterial = i;
							break;
						}
			}

			// Rest of the line contains the filename of a material library
			if (!strncmp(szString, MTL_LIB_ID, sizeof(MTL_LIB_ID)))
			{

				// Read the rest of the line (the complete filename)
				GetTokenParameter(szString, sizeof(szString), hFile);
				// Append material library filename to the model's base path
				char szLibraryFile[_MAX_PATH];
				strcpy_s(szLibraryFile, szBasePath);
				strcat_s(szLibraryFile, szString);
				// Load the material library
				LoadMaterialLib(szLibraryFile, pMaterials,
					&CurrentIndex.iMaterialCount, szBasePath);
			}
		}
	}

	// Close OBJ file
	fclose(hFile);

	////////////////////////////////////////////////////////////////////////
	// Arrange and render the model data
	////////////////////////////////////////////////////////////////////////

	// Sort the faces by material to minimize state changes
	if (!materialList.empty())
	{
		qsort(pFaces, OBJInfo.iFaceCount, sizeof(Face), CompareFaceByMaterial);

		// Copiar el vector pMaterials a un vector fixe vMaterials.
		for (int i = 0; i < numMaterials; i++) vMaterials[i] = pMaterials[i];
	}

	// Render all faces into a VAO
	loadToVAOList(pFaces, OBJInfo.iFaceCount, pMaterials);

	////////////////////////////////////////////////////////////////////////
	// Free structures that hold the model data
	////////////////////////////////////////////////////////////////////////

	// Remove vertices, normals, materials and texture coordinates
	delete[] pVertices;	pVertices = 0;
	delete[] pNormals;		pNormals = 0;
	delete[] pTexCoords;	pTexCoords = 0;
	delete[] pMaterials;	pMaterials = 0;

	// Remove face array
	for (i = 0; i < (int)OBJInfo.iFaceCount; i++)
	{
		// Delete every pointer in the face structure
		delete[] pFaces[i].pNormals;	pFaces[i].pNormals = 0;
		delete[] pFaces[i].pTexCoords;	pFaces[i].pTexCoords = 0;
		delete[] pFaces[i].pVertices;  pFaces[i].pVertices = 0;
	}
	delete[] pFaces;
	pFaces = 0;

	////////////////////////////////////////////////////////////////////////
	// Success
	////////////////////////////////////////////////////////////////////////
	return 0;
}

// Metodos Publicos de Limpieza Grafica

void _stdcall Object::ClearMeshList()
{
	GLint i;
	for (i = 0, size = meshList.size(); i < size; i++)
	{
		meshList[i].ClearMesh();
	}
}

// Destructors

_stdcall Object::~Object()
{
	ClearMeshList();
}

// Metodos Privados de Inicializacion

bool _stdcall Object::LoadMaterialLib(const char szFileName[], unsigned int* iCurMaterialIndex, char szBasePath[])
{
	////////////////////////////////////////////////////////////////////////
	// Loads a material library file (.mtl)
	////////////////////////////////////////////////////////////////////////

	char szString[MAX_STR_SIZE];	// Buffer used while reading the file
	bool bFirstMaterial = TRUE;		// Only increase index after first material has been set
	FILE* hFileT = NULL;
	int errno;

	////////////////////////////////////////////////////////////////////////
	// Open library file
	////////////////////////////////////////////////////////////////////////

	errno = fopen_s(&hFileT, szFileName, "r");
	// Success ?
	if (errno != 0)
		return FALSE;

	////////////////////////////////////////////////////////////////////////
	// Read all material definitions
	////////////////////////////////////////////////////////////////////////

	// Quit reading when end of file has been reached
	while (!feof(hFileT))
	{
		// Get next string
		ReadNextString(szString, hFileT);

		// Is it a "new material" identifier ?
		if (!strncmp(szString, NEW_MTL_ID, sizeof(NEW_MTL_ID)))
		{
			// Only increase index after first material has been set
			if (bFirstMaterial == TRUE)
				// First material has been set
				bFirstMaterial = FALSE;
			else
				// Use next index
				(*iCurMaterialIndex)++;
			// Read material name
			GetTokenParameter(szString, sizeof(szString), hFileT);
			// Store material name in the structure
			strcpy_s(pMaterials[*iCurMaterialIndex].szName, szString);
			// Inicialitzar valors registre materials
			materialList.emplace_back();
		}
		else
		{
			// Cada material parsea sus datos
			materialList.back().LoadMaterial(hFileT, szBasePath);
		}
	}

	fclose(hFileT);

	// Increment index cause LoadMaterialLib() assumes that the passed
	// index is always empty
	(*iCurMaterialIndex)++;

	numMaterials = *iCurMaterialIndex;
	return TRUE;
}

void _stdcall Object::loadToVAOList(const Face* pFaces, const unsigned int iFaceCount)
{
	////////////////////////////////////////////////////////////////////////
	// Load a list of faces into a VAO
	////////////////////////////////////////////////////////////////////////
	int i, j;
	float fNormal[3];
	int iPreviousMaterial = -1;
	double color[4] = { 1.0F, 0.0f, 0.0f, 1.0f };

	// VAO
	meshList.emplace_back();

	std::vector <double> vertices, colors, normals, textures;		// Definicio vectors dinamics per a vertexs i colors 
	vertices.resize(0);		colors.resize(0);	normals.resize(0);		textures.resize(0);// Reinicialitzar vectors

	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

	meshList.back().ClearMesh();

	// Obtenir color actual definit en OpenGL amb glColor();
	GLfloat cColor[4];
	glGetFloatv(GL_CURRENT_COLOR, cColor);

	// Activate automatic texture coord generation if no coords loaded
	if (!pFaces[0].pTexCoords) GenTexCoords();

	// Use default material if no materials are loaded
	if (materialList.empty()) UseMaterial(NULL);

	// Process all faces
	for (i = 0; i < (int)iFaceCount; i++)
	{	// Process all vertices as triangles
		for (j = 1; j < (int)pFaces[i].iNumVertices - 1; j++)
		{
			nv = vertices.size();	// Tamany del vector vertices en elements.
			if (!materialList.empty())
			{	// Set material (if it differs from the previous one)
				if (iPreviousMaterial != (int)pFaces[i].iMaterialIndex)
				{
					// Canvi de material per a les cares del fitxer OBJ
					if (nv > 0) {
						// Creacio d'un VAO i un VBO i carrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
						meshList.back().load_TRIANGLES_VAO(vertices, normals, colors, textures);	
						meshList.emplace_back();
						index_VAO = index_VAO + 1;
						vertices.resize(0);		colors.resize(0);	normals.resize(0);		textures.resize(0);// Reinicialitzar vectors
					}
					materialIndexes.emplace_back(pFaces[i].iMaterialIndex);
					iPreviousMaterial = pFaces[i].iMaterialIndex;
				}
			}
			// -----------VERTEX 0
			// Set vertex normal (if vertex normals are specified)
			if (pFaces[i].pNormals)
			{
				normals.push_back(pFaces[i].pNormals[0].fX);
				normals.push_back(pFaces[i].pNormals[0].fY);
				normals.push_back(pFaces[i].pNormals[0].fZ);
			}

			else {	// Calculate and set face normal if no vertex normals are specified
				GetFaceNormal(fNormal, &pFaces[i]);
				normals.push_back(fNormal[0]);
				normals.push_back(fNormal[1]);
				normals.push_back(fNormal[2]);
			}

			// Set texture coordinates (if any specified)
			if (!materialList.empty())
			{
				if (materialList[pFaces[i].iMaterialIndex].iTextureID != 0)
				{
					if (pFaces[i].pTexCoords)
					{
						textures.push_back(pFaces[i].pTexCoords[0].fX);
						textures.push_back(pFaces[i].pTexCoords[0].fY);
					}
					else {
						textures.push_back(0.0);
						textures.push_back(0.0);
					}
				}
				else {
					textures.push_back(0.0);
					textures.push_back(0.0);
				}
			}
			else {
				textures.push_back(0.0);
				textures.push_back(0.0);
			}

			// Set COLOR: Any materials loaded ?
			if (!materialList.empty())
			{	// Set Color
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[0]);
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[1]);
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[2]);
				colors.push_back(1.0);
			}
			else {	// Set Color per defecte
				colors.push_back(cColor[0]);
				colors.push_back(cColor[1]);
				colors.push_back(cColor[2]);
				colors.push_back(cColor[3]);
			}

			// Set VERTEX
			vertices.push_back(pFaces[i].pVertices[0].fX);
			vertices.push_back(pFaces[i].pVertices[0].fY);
			vertices.push_back(pFaces[i].pVertices[0].fZ);

			// -----------VERTEX j
// Set vertex normal (if vertex normals are specified)
			if (pFaces[i].pNormals)
			{
				normals.push_back(pFaces[i].pNormals[j].fX);
				normals.push_back(pFaces[i].pNormals[j].fY);
				normals.push_back(pFaces[i].pNormals[j].fZ);
			}

			else {	// Calculate and set face normal if no vertex normals are specified
				GetFaceNormal(fNormal, &pFaces[i]);
				normals.push_back(fNormal[0]);
				normals.push_back(fNormal[1]);
				normals.push_back(fNormal[2]);
			}

			// Set texture coordinates (if any specified)
			if (!materialList.empty())
			{
				if (materialList[pFaces[i].iMaterialIndex].iTextureID != 0)
				{
					if (pFaces[i].pTexCoords)
					{
						textures.push_back(pFaces[i].pTexCoords[j].fX);
						textures.push_back(pFaces[i].pTexCoords[j].fY);
					}
					else {
						textures.push_back(0.0);
						textures.push_back(0.0);
					}
				}
				else {
					textures.push_back(0.0);
					textures.push_back(0.0);
				}
			}
			else {
				textures.push_back(0.0);
				textures.push_back(0.0);
			}

			// Set COLOR: Any materials loaded ?
			if (!materialList.empty())
			{	// Set Color
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[0]);
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[1]);
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[2]);
				colors.push_back(1.0);
			}
			else {	// Set Color per defecte
				colors.push_back(cColor[0]);
				colors.push_back(cColor[1]);
				colors.push_back(cColor[2]);
				colors.push_back(cColor[3]);
			}

			// Set VERTEX
			vertices.push_back(pFaces[i].pVertices[j].fX);
			vertices.push_back(pFaces[i].pVertices[j].fY);
			vertices.push_back(pFaces[i].pVertices[j].fZ);

			// -----------VERTEX j+1
// Set vertex normal (if vertex normals are specified)
			if (pFaces[i].pNormals)
			{
				normals.push_back(pFaces[i].pNormals[j + 1].fX);
				normals.push_back(pFaces[i].pNormals[j + 1].fY);
				normals.push_back(pFaces[i].pNormals[j + 1].fZ);
			}
			else {	// Calculate and set face normal if no vertex normals are specified
				GetFaceNormal(fNormal, &pFaces[i]);
				normals.push_back(fNormal[0]);
				normals.push_back(fNormal[1]);
				normals.push_back(fNormal[2]);
			}

			// Set texture coordinates (if any specified)
			if (!materialList.empty())
			{
				if (materialList[pFaces[i].iMaterialIndex].iTextureID != 0)
				{
					if (pFaces[i].pTexCoords)
					{
						textures.push_back(pFaces[i].pTexCoords[j + 1].fX);
						textures.push_back(pFaces[i].pTexCoords[j + 1].fY);
					}
					else {
						textures.push_back(0.0);
						textures.push_back(0.0);
					}
				}
				else {
					textures.push_back(0.0);
					textures.push_back(0.0);
				}
			}
			else {
				textures.push_back(0.0);
				textures.push_back(0.0);
			}

			// Set COLOR: Any materials loaded ?
			if (!materialList.empty())
			{	// Set Color
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[0]]);
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[1]]);
				colors.push_back(materialList[pFaces[i].iMaterialIndex].fDiffuse[2]]);
				colors.push_back(1.0);
			}
			else {	// Set Color per defecte
				colors.push_back(cColor[0]);
				colors.push_back(cColor[1]);
				colors.push_back(cColor[2]);
				colors.push_back(cColor[3]);
			}

			// Set VERTEX
			vertices.push_back(pFaces[i].pVertices[j + 1].fX);
			vertices.push_back(pFaces[i].pVertices[j + 1].fY);
			vertices.push_back(pFaces[i].pVertices[j + 1].fZ);
		}
	}

	// ----------------------- VAO
	nv = vertices.size();	// Tamany del vector vertices en elements.

	// Numero de Materials del fitxer OBJ
	numMaterials = index_VAO + 1;
}

void _stdcall Object::GetFaceNormal(float fNormalOut[], const Face* pFace)
{
	////////////////////////////////////////////////////////////////////////
	// Calculate normal for a given face
	////////////////////////////////////////////////////////////////////////

	// Only faces with at least 3 vertices can have a normal
	if (pFace->iNumVertices < 3)
	{
		// Clear the normal and exit
		memset(fNormalOut, 0, sizeof(fNormalOut));
		return;
	}

	// Two vectors
	float v1[3] = { 0.0,0.0,0.0 }, v2[3] = { 0.0,0.0,0.0 };

	// Calculate two vectors from the three points
	v1[x] = pFace->pVertices[0].fX - pFace->pVertices[1].fX;
	v1[y] = pFace->pVertices[0].fY - pFace->pVertices[1].fY;
	v1[z] = pFace->pVertices[0].fZ - pFace->pVertices[1].fZ;

	v2[x] = pFace->pVertices[1].fX - pFace->pVertices[2].fX;
	v2[y] = pFace->pVertices[1].fY - pFace->pVertices[2].fY;
	v2[z] = pFace->pVertices[1].fZ - pFace->pVertices[2].fZ;

	// Take the cross product of the two vectors to get the normal vector
	fNormalOut[x] = v1[y] * v2[z] - v1[z] * v2[y];
	fNormalOut[y] = v1[z] * v2[x] - v1[x] * v2[z];
	fNormalOut[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Calculate the length of the vector		
	float length = (float)sqrt((fNormalOut[x] * fNormalOut[x]) +
		(fNormalOut[y] * fNormalOut[y]) +
		(fNormalOut[z] * fNormalOut[z]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	fNormalOut[x] /= length;
	fNormalOut[y] /= length;
	fNormalOut[z] /= length;
}

void _stdcall Object::ParseFaceString(char szFaceString[], Face* FaceOut, const Vector3D* pVertices, const Vector3D* pNormals, const Vector2D* pTexCoords, const unsigned int iMaterialIndex)
{
	////////////////////////////////////////////////////////////////////////
	// Convert face string from the OBJ file into a face structure
	////////////////////////////////////////////////////////////////////////

	int i;
	int iVertex = 0, iTextureCoord = 0, iNormal = 0;

	// Pointer to the face string. Will be incremented later to
	// advance to the next triplet in the string.
	char* pFaceString = szFaceString;

	// Save the string positions of all triplets
	int iTripletPos[MAX_VERTICES];
	int iCurTriplet = 0;

	// Init the face structure
	memset(FaceOut, 0, sizeof(Face));

	////////////////////////////////////////////////////////////////////////
	// Get number of vertices in the face
	////////////////////////////////////////////////////////////////////////

	// Loop trough the whole string
	for (i = 0; i < (int)strlen(szFaceString); i++)
	{
		// Each triplet is separated by spaces
		if (szFaceString[i] == ' ')
		{
			// One more vertex
			FaceOut->iNumVertices++;
			// Save position of triplet
			iTripletPos[iCurTriplet] = i;
			// Next triplet
			iCurTriplet++;
		}
	}

	// Face has more vertices than spaces that separate them
	FaceOut->iNumVertices++;

	////////////////////////////////////////////////////////////////////////
	// Allocate space for structures that hold the face data
	////////////////////////////////////////////////////////////////////////

	// Vertices
	FaceOut->pVertices = new Vector3D[FaceOut->iNumVertices];

	// Allocate space for normals and texture coordinates only if present
	if (pNormals)
		FaceOut->pNormals = new Vector3D[FaceOut->iNumVertices];
	if (pTexCoords)
		FaceOut->pTexCoords = new Vector2D[FaceOut->iNumVertices];

	////////////////////////////////////////////////////////////////////////
	// Copy vertex, normal, material and texture data into the structure
	////////////////////////////////////////////////////////////////////////

	// Set material
	FaceOut->iMaterialIndex = iMaterialIndex;

	// Process per-vertex data
	for (i = 0; i < (int)FaceOut->iNumVertices; i++)
	{
		// Read one triplet from the face string

		// Are vertices, normals and texture coordinates present ?
		if (pNormals && pTexCoords)
			// Yes
			sscanf_s(pFaceString, "%i/%i/%i",
				&iVertex, &iTextureCoord, &iNormal);
		else if (pNormals && !pTexCoords)
			// Vertices and normals but no texture coordinates
			sscanf_s(pFaceString, "%i//%i", &iVertex, &iNormal);
		else if (pTexCoords && !pNormals)
			// Vertices and texture coordinates but no normals
			sscanf_s(pFaceString, "%i/%i", &iVertex, &iTextureCoord);
		else
			// Only vertices
			sscanf_s(pFaceString, "%i", &iVertex);

		// Copy vertex into the face. Also check for normals and texture 
		// coordinates and copy them if present.
		memcpy(&FaceOut->pVertices[i], &pVertices[iVertex - 1],
			sizeof(Vector3D));
		if (pTexCoords)
			memcpy(&FaceOut->pTexCoords[i],
				&pTexCoords[iTextureCoord - 1], sizeof(Vector2D));
		if (pNormals)
			memcpy(&FaceOut->pNormals[i],
				&pNormals[iNormal - 1], sizeof(Vector3D));

		// Set string pointer to the next triplet
		pFaceString = &szFaceString[iTripletPos[i]];
	}
}

// Metodos Adicionales Externos

static int CompareFaceByMaterial(const void* Arg1, const void* Arg2)
{
	////////////////////////////////////////////////////////////////////////
	// Callback function for comparing two faces with qsort
	////////////////////////////////////////////////////////////////////////

	// Cast the void pointers to faces
	Face* Face1 = (Face*)Arg1;
	Face* Face2 = (Face*)Arg2;

	// Greater
	if (Face1->iMaterialIndex > Face2->iMaterialIndex)
		return 1;

	// Less
	if (Face1->iMaterialIndex < Face2->iMaterialIndex)
		return -1;

	// Equal
	return 0;
}
