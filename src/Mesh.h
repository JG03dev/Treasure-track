//
// Created by jg03dev on 10/9/23.
//

#pragma once
#include "stdafx.h"


class Mesh {
public:
    // Constructors

    Mesh();

    // Metodes Publics de Inicialitzacio

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void load_TRIANGLES_VAO(std::vector <double> vertices, std::vector <double> normals, std::vector <double> colors, std::vector <double> textures);

    // Metodes Publics Funcionals

    void RenderMesh();

    // Metodes Publics de Neteja

    void ClearMesh();

    // Destructors

    ~Mesh();

private:
    // Atributs Privats

    GLuint VAO, VBO, IBO;
    GLsizei indexCount, vertexCount;
};
