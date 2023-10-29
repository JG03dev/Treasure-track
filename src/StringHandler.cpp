#include "stdafx.h"

//TODO: Poner estos defines en la classe objeto
// OBJ File string indentifiers
#define VERTEX_ID		 "v"
#define TEXCOORD_ID		 "vt"
#define NORMAL_ID		 "vn"
#define FACE_ID			 "f"
#define COMMENT_ID		 "#"
#define MTL_LIB_ID		 "mtllib"
#define USE_MTL_ID		 "usemtl"


// MTL File string indentifiers
#define NEW_MTL_ID		 "newmtl"
#define MTL_TEXTURE_ID   "map_Kd"
#define MTL_AMBIENT_ID	 "Ka"
#define MTL_DIFFUSE_ID	 "Kd"
#define MTL_SPECULAR_ID	 "Ks"
#define MTL_SHININESS_ID "Ns"

#define COMMENT_ID		 "#"

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

#define MAX_STR_SIZE 1024

fpos_t ReadNextString(char szString[], FILE* hStream)
{
	////////////////////////////////////////////////////////////////////////
	// Read the next string that isn't a comment
	////////////////////////////////////////////////////////////////////////

	bool bSkipLine = FALSE;	// Skip the current line ?
	int nScanReturn = 0;	// Return value of fscanf
	fpos_t pos;
	// Skip all strings that contain comments
	do
	{
		// Read new string
		fgetpos(hStream, &pos);
		nScanReturn = fscanf(hStream, "%s", szString);
		// Is rest of the line a comment ?
		if (!strncmp(szString, COMMENT_ID, sizeof(COMMENT_ID)))
		{
			// Skip the rest of the line
			fgets(szString, sizeof(szString), hStream);
			bSkipLine = TRUE;
		}
		else bSkipLine = FALSE;
	} while (bSkipLine == TRUE);
	return pos;
}

void GetTokenParameter(char szString[], const unsigned int iStrSize, FILE *hFile)
{
////////////////////////////////////////////////////////////////////////
// Read the parameter of a token, remove space and newline character
////////////////////////////////////////////////////////////////////////

// Read the parameter after the token
	fgets(szString, iStrSize, hFile);

// Remove space before the token			
	strcpy(szString, &szString[1]);

// Remove newline character after the token
	szString[strlen(szString) - 1] = char('\0');
}

void MakePath(char szFileAndPath[])
{
////////////////////////////////////////////////////////////////////////
// Rips the filenames out of a path and adds a slash (if needed)
////////////////////////////////////////////////////////////////////////
	
// Get string length
	int iNumChars = (int)strlen(szFileAndPath);

	// Loop from the last to the first char
	for (int iCurChar=iNumChars-1; iCurChar>=0; iCurChar--)
	{
		// If the current char is a slash / backslash
		if (szFileAndPath[iCurChar] == char('\\') ||
			szFileAndPath[iCurChar] == char('/'))
		{
			// Terminate the the string behind the slash / backslash
			szFileAndPath[iCurChar + 1] = char('\0');
			return;
		}
	}

// No slash there, set string length to zero
	szFileAndPath[0] = char('\0');
}

void GetFileInfo(FILE *hStream, OBJFileInfo *Info, const char szConstBasePath[])
{
////////////////////////////////////////////////////////////////////////
// Read the count of all important identifiers out of the given stream
////////////////////////////////////////////////////////////////////////

	char szString[MAX_STR_SIZE]; // Buffer for reading the file
	char szBasePath[_MAX_PATH];	 // Needed to append a filename to the base path
	
	FILE *hMaterialLib = NULL;
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
			Info->iVertexCount++;	
// Texture coordinate ?
		if (!strncmp(szString, TEXCOORD_ID, sizeof(TEXCOORD_ID)))
			Info->iTexCoordCount++;
		// Vertex normal ?
		if (!strncmp(szString, NORMAL_ID, sizeof(NORMAL_ID)))
			Info->iNormalCount++;		
		// Face ?
		if (!strncmp(szString, FACE_ID, sizeof(FACE_ID)))
			Info->iFaceCount++;

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
			errno = fopen_s(&hMaterialLib,szBasePath, "r");
			// Success ?
			if (!errno) //(hMaterialLib)
			{
				// Quit reading when end of file has been reached
				while (!feof(hMaterialLib))
				{
					// Read next string
					fscanf(hMaterialLib, "%s" ,szString);
					// Is it a "new material" identifier ?
					if (!strncmp(szString, NEW_MTL_ID, sizeof(NEW_MTL_ID)))
						// One more material defined
						Info->iMaterialCount++;		
				}
				// Close material library
				fclose(hMaterialLib);
			}
		}
			
		// Clear string two avoid counting something twice
		memset(szString, '\0', sizeof(szString));
	}
}

void GenTexCoords()
{
////////////////////////////////////////////////////////////////////////
// Set up the automatic texture coord generation
////////////////////////////////////////////////////////////////////////

	float fS[4] = { -0.1f, 0.0f, 0.0f, 0.0f };
	float fT[4] = { 0.0f, 0.0f, -0.1f, 0.0f };

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexGenfv(GL_S, GL_OBJECT_PLANE, fS);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, fT);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
}