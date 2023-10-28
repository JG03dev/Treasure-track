//
// Created by jg03dev on 10/9/23.
//

#include "Mesh.h"

// Constructors

Mesh::Mesh() {
    VAO = 0;
    VBO = 0;
    IBO = 0;
    vertexCount = 0;
    indexCount = 0;
}

// Metodes Publics de Inicialitzacio

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices){
    indexCount = numOfIndices;

    // Creating and binding VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Creating and binding IBO
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

    // Creating and binding VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);
    /*
    * STATIC_DRAW: the points of our triangle will most likely be static (they won't really move)
    * DINAMYC_DRAW: more complex to use, we suppose the points of our triangle will move at some point.
    */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::load_TRIANGLES_VAO(std::vector<double> vertices, std::vector<double> normals, std::vector<double> colors, std::vector<double> textures)
{

    std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.


// Create Vertex Array Object (VAO) for 3D Model Cube
    glGenVertexArrays(1, &VAO);

    // Create vertex buffer objects for 3D Model attributes in the VAO
    glGenBuffers(1, &VBO);

    // Bind our Vertex Array Object as the current used object
    glBindVertexArray(VAO);

    // Bind our Vertex Buffer Object as the current used object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);

    // Position Vertex attributes
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offset
    glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

    // Normal Vertex Attributes
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), normals.size() * sizeof(double), &normals[0]);	// Copy normal data to VBO starting from 0 offest
    glEnableVertexAttribArray(1);											// Enable attribute index 1 as being used (normals)
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

    // Texture Coordinates Vertex Attributes
    glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(double), textures.size() * sizeof(double), &textures[0]);	// Copy normal data to VBO starting from 0 offest
    glEnableVertexAttribArray(2);												// Enable attribute index 2 as being used (texture coordinates)
    glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (GLvoid*)((vertices.size() + normals.size()) * sizeof(double)));						// Specify that our color data is going into attribute index 0 and contains 3 double

    // Color Vertex Attributes
    glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size()) * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
    glEnableVertexAttribArray(3);												// Enable attribute index 3 as being used
    glVertexAttribPointer(3, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)((vertices.size() + normals.size() + textures.size()) * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

    // Unbind VAO, to prevent bugs
    glBindVertexArray(0);

    vertexCount = (int)vertices.size() / 3;
    indexCount = 0;
}

// Metodes Publics Funcionals

void Mesh::RenderMesh(){
    glBindVertexArray(VAO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Metodes Publics de Neteja

void Mesh::ClearMesh(){
    glBindVertexArray(VAO); //glBindVertexArray(vaoId);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
    vertexCount = 0;
}

// Destructors

Mesh::~Mesh() {
    ClearMesh();
}