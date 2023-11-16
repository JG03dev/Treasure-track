#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>
#include <string>
#include <unordered_map>

#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

class Base2D {
public:
	/** Render Data */
	std::vector<Pixel>         vertices;
	std::vector<unsigned int>  indices;
	std::vector<Texture>       textures;

	/** Methods */
	Base2D();
	~Base2D();

	void Draw(Shader& shader);
	void AddTexture(unsigned int tid);
	void AddTexture(const std::string path, TextureType type, bool gamma = false);
	void DeleteBuffers();

	unsigned int VBO() { return vbo; }
	unsigned int VAO() { return vao; }
	unsigned int EBO() { return ebo; }

protected:
	/** Render Data */
	unsigned int vbo, ebo, vao;

	/** Methods */
	void setup();
};

class Base3D {
public:
	/** Render Data */
	std::vector<Vertex>        vertices;
	std::vector<unsigned int>  indices;
	std::vector<Texture>       textures;

	/** Methods */
	Base3D();
	~Base3D();

	void Draw(Shader& shader);
	void AddTexture(unsigned int tid);
	void AddTexture(const std::string path, TextureType type, bool gamma = false);
	void DeleteBuffers();

	unsigned int VBO() { return vbo; }
	unsigned int VAO() { return vao; }
	unsigned int EBO() { return ebo; }

protected:
	/** Render Data */
	unsigned int vbo, ebo, vao;

	/** Methods */
	void setup();
};

#endif