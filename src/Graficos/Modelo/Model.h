#pragma once

#include "../../Encabezados/stdafx.h"

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

class Model
{
public:
	Model();

	Model(Material& m) : materialModel(m) {};
	Model(GLfloat sIntensity, GLfloat shine) : materialModel(sIntensity, shine) {}

	void LoadModel(const std::string& fileName, std::string name);
	void RenderModel(Shader& s);
	void ClearModel();

	std::string GetName() { return m_name; }

	~Model();

	std::vector<Mesh*> meshList;

private:

	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);

	
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
	std::string m_name;

	//TODO: Manage to do this as a constant
	const char* PLAIN_PATH = "../../../Assets/plain.png"; //Path for default imag

	//EXTRA: Material custom a�adido
	Material materialModel;
};

