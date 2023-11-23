#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include "stdafx.h"
#include "Texture.h"
#include "Shader.h"

struct Pixel {
	glm::vec2 position;
	glm::vec2 texCoords;
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class Mesh {

public:
	/** Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	/** Methods */
	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	//~Mesh();

	void Draw(Shader& shader);
	void DeleteBuffers();

	GLuint VAO() const { return vao; }
	GLuint VBO() const { return vbo; }
	GLuint EBO() const { return ebo; }

private:
	/** Render Data */
	GLuint vbo, ebo, vao;

	/** Methods */
	void setup();
};

#endif