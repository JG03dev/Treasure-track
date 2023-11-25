#pragma once

#include "../../Encabezados/stdafx.h"

class Mesh
{
public:
	Mesh();

	void CreateMesh(std::vector<GLfloat> vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	const std::vector<GLfloat>* GetVertices() { return &raw_vertices; }

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

	// Datos para las fisicas
	std::vector<GLfloat> raw_vertices;
};

