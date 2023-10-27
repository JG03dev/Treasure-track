#include "Object.h"

// OBJ File string indentifiers
#define VERTEX_ID		 "v"
#define TEXCOORD_ID		 "vt"
#define NORMAL_ID		 "vn"
#define FACE_ID			 "f"
#define COMMENT_ID		 "#"
#define MTL_LIB_ID		 "mtllib"
#define USE_MTL_ID		 "usemtl"

struct OBJFileInfo
{
	unsigned int iVertexCount;
	unsigned int iTexCoordCount;
	unsigned int iNormalCount;
	unsigned int iFaceCount;
	unsigned int iMaterialCount;
};

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

	// Vertices and faces
	Vector3D* pVertices = new Vector3D[OBJInfo.iVertexCount];
	Face* pFaces = new Face[OBJInfo.iFaceCount];

	// Allocate space for optional model data only if present.
	Vector3D* pNormals = 0;
	Vector2D* pTexCoords = 0;
	Material* pMaterials = 0;
	if (OBJInfo.iNormalCount)
		pNormals = new Vector3D[OBJInfo.iNormalCount];
	if (OBJInfo.iTexCoordCount)
		pTexCoords = new Vector2D[OBJInfo.iTexCoordCount];
	if (OBJInfo.iMaterialCount)
		pMaterials = new Material[OBJInfo.iMaterialCount];

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
			this->mesh.AddVertex(hFile);
		}

		// Next two elements are floats of a texture coordinate
		if (!strncmp(szString, TEXCOORD_ID, sizeof(TEXCOORD_ID)))
		{
			this->mesh.AddTexCoord(hFile);
		}

		// Next three elements are floats of a vertex normal
		if (!strncmp(szString, NORMAL_ID, sizeof(NORMAL_ID)))
		{
			this->mesh.AddNormal(hFile);
		}

		// Rest of the line contains face information
		if (!strncmp(szString, FACE_ID, sizeof(FACE_ID)))
		{
			this->mesh.AddFace(hFile, iCurMaterial);
		}

		// Process material information only if needed
		if (pMaterials)
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
	if (pMaterials)
	{
		qsort(pFaces, OBJInfo.iFaceCount, sizeof(Face), CompareFaceByMaterial);

		// Identificar el número de llista donat com a paràmetre a la variable local m_iDisplayList
		//m_iDisplayList = iDisplayList;
		//if (m_iDisplayList>0) glDeleteLists(m_iDisplayList, 1);

		// Copiar el vector pMaterials a un vector fixe vMaterials.
//		for (int i = 0; i < sizeof(pMaterials); i++) vMaterials[i] = pMaterials[i];	
		for (int i = 0; i < numMaterials; i++) vMaterials[i] = pMaterials[i];
	}

	// Render all faces into a VAO
		//auxVAO = RenderToVAOList(pFaces, OBJInfo.iFaceCount, pMaterials, prim_Id);
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

void _stdcall Object::GetFileInfo(FILE* hStream, OBJFileInfo* Info,
	const char szConstBasePath[])
{
	////////////////////////////////////////////////////////////////////////
	// Read the count of all important identifiers out of the given stream
	////////////////////////////////////////////////////////////////////////

	char szString[MAX_STR_SIZE]; // Buffer for reading the file
	char szBasePath[_MAX_PATH];	 // Needed to append a filename to the base path

	FILE* hMaterialLib = NULL;
	int errno;

	// Init structure
	memset(Info, 0, sizeof(OBJFileInfo));

	// Rollback the stream
	rewind(hStream);

	// Quit reading when end of file has been reached
	while (!feof(hStream))
	{
		// Get next string
		ReadNextString(szString, hStream);

		// Vertex ?
		if (!strncmp(szString, VERTEX_ID, sizeof(VERTEX_ID)))
			this->mesh.CountVertex();
		// Texture coordinate ?
		if (!strncmp(szString, TEXCOORD_ID, sizeof(TEXCOORD_ID)))
			this->mesh.CountTexCoord();
		// Vertex normal ?
		if (!strncmp(szString, NORMAL_ID, sizeof(NORMAL_ID)))
			this->mesh.CountNormal();
		// Face ?
		if (!strncmp(szString, FACE_ID, sizeof(FACE_ID)))
			this->mesh.CountFace();

		// Material library definition ?
		if (!strncmp(szString, MTL_LIB_ID, sizeof(MTL_LIB_ID)))
		{
			// Read the filename of the library
			GetTokenParameter(szString, sizeof(szString), hStream);
			// Copy the model's base path into a none-constant string
			strcpy_s(szBasePath, szConstBasePath);
			// Append material library filename to the model's base path
			strcat_s(szBasePath, szString);

			// Open the library file
//			FILE *hMaterialLib = fopen(szBasePath, "r");
			errno = fopen_s(&hMaterialLib, szBasePath, "r");
			// Success ?
			if (!errno) //(hMaterialLib)
			{
				// Quit reading when end of file has been reached
				while (!feof(hMaterialLib))
				{

					// Read next string
					fscanf(hMaterialLib, "%s", szString);
					// Is it a "new material" identifier ?
					if (!strncmp(szString, NEW_MTL_ID, sizeof(NEW_MTL_ID))) {
						this->vMaterials.emplace_back();
						this->vMaterials.back();
						// One more material defined
						Info->iMaterialCount++;
					}
						
				}
				// Close material library
				fclose(hMaterialLib);
			}
		}

		// Clear string two avoid counting something twice
		memset(szString, '\0', sizeof(szString));
	}
}
