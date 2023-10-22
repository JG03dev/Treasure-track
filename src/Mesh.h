//
// Created by jg03dev on 10/9/23.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>


class Mesh {
public:
    Mesh();

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void RenderMesh();
    void ClearMesh();
    void loadModel(std::string path);

    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};
