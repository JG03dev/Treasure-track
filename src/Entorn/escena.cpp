#include "stdafx.h"
#include "material.h"
#include "visualitzacio.h"
#include "escena.h"

// dibuixa_EscenaGL: Dibuix de l'escena amb comandes GL
void dibuixa_EscenaGL(GLuint sh_programID, bool eix, GLuint axis_Id, CMask3D reixa, CPunt3D hreixa, char objecte, 
			CColor col_object, bool sw_mat[5],
			bool textur, GLuint texturID[NUM_MAX_TEXTURES], bool textur_map, bool flagInvertY,
			int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC, bool dib_TFrenet,
			COBJModel* objecteOBJ,
			glm::mat4 MatriuVista, glm::mat4 MatriuTG)
{
	float altfar = 0;
	GLint npunts = 0, nvertexs = 0;
	int i, j, k;
	GLdouble tras[3] = { 0.0,0.0,0.0 }; //Sierpinski Sponge
	CColor color_vermell = { 0.0,0.0,0.0,1.0 }, color_Mar = { 0.0,0.0,0.0,0.0 };
	bool sw_material[5] = { 0.0,0.0,0.0,0.0,0.0 };
	
// Matrius de Transformació
	glm::mat4 NormalMatrix(1.0), ModelMatrix(1.0), TransMatrix(1.0), ScaleMatrix(1.0), RotMatrix(1.0);

// VAO
	CVAO objectVAO = { 0,0,0,0,0 };
	objectVAO.vaoId = 0;	objectVAO.vboId = 0;	objectVAO.eboId = 0;	 objectVAO.nVertexs = 0; objectVAO.nIndices = 0;

	tras[0] = 0.0;	tras[1] = 0.0; tras[2] = 0.0;
	color_vermell.r = 1.0;	color_vermell.g = 0.0; color_vermell.b = 0.0; color_vermell.a = 1.0;
	sw_material[0] = false;	sw_material[1] = true; sw_material[2] = true; sw_material[3] = false;	sw_material[4] = true;

// Shader Visualització Objectes
	glUseProgram(sh_programID);

// Parametrització i activació/desactivació de textures
	if (texturID[0] != -1) SetTextureParameters(texturID[0], true, true, textur_map, false);
	if (textur) {	glUniform1i(glGetUniformLocation(sh_programID, "textur"), GL_TRUE); //glEnable(GL_TEXTURE_2D);
					glUniform1i(glGetUniformLocation(sh_programID, "modulate"), GL_TRUE); //glEnable(GL_MODULATE);
				}
		else {	glUniform1i(glGetUniformLocation(sh_programID, "textur"), GL_FALSE); //glDisable(GL_TEXTURE_2D);
				glUniform1i(glGetUniformLocation(sh_programID, "modulate"), GL_FALSE); //glDisable(GL_MODULATE);
			}
	glUniform1i(glGetUniformLocation(sh_programID, "flag_invert_y"), flagInvertY);

// Attribute Locations must be setup before calling glLinkProgram()
	glBindAttribLocation(sh_programID, 0, "in_Vertex");		// Vèrtexs
	glBindAttribLocation(sh_programID, 1, "in_Normal");		// Normals
	glBindAttribLocation(sh_programID, 2, "in_TexCoord");	// Textura
	glBindAttribLocation(sh_programID, 3, "in_Color");		// Color

// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	SeleccionaColorMaterial(sh_programID, col_object, sw_mat);

	// Dibuix de l'objecte OBJ

	ModelMatrix = MatriuTG;
	// Pas ModelView Matrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);

	// Definir característiques material de cada punt
	SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
	// Objecte OBJ: Dibuix de l'objecte OBJ amb textures amb varis VAO's, un per a cada material.
	objecteOBJ->draw_TriVAO_OBJ(sh_programID);	// Dibuixar VAO a pantalla
}