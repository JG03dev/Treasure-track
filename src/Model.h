#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "stdafx.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Model
{
public:
	/** Methods */
	Model(std::string path, bool gamma = false);
	~Model();
	void Draw(Shader& shader);

	/** Model Data */
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

private:
	/** Model Data */
	std::string directory;
	bool gammaCorrection;

	/** Methods */
	void loadModel(std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType aiTexType, TextureType type);
};

#endif