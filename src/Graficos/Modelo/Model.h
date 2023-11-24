#pragma once

#include "../../Encabezados/stdafx.h"

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName, std::string name);
	void RenderModel();
	void ClearModel();

	~Model();

private:

	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
	std::string m_name;
};

