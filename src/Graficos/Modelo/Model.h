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
	Model(GLfloat sIntensity, GLfloat shine, std::string path, std::string name ,int nIns=1) :
		materialModel(sIntensity, shine), modelPath(path), m_name(name), nInstances(nIns), isLoaded(false) {}

	void LoadModel(const std::string& fileName, std::string name);
	void Load(); // Loads if name and fileName are already set
	void RenderModel(Shader& s);
	void ClearModel();

	// setters
	void setInstances(int nIns) { nInstances = nIns; }

	// getters
	int getNInstances() { return nInstances; }
	std::string GetName() { return m_name; }
	bool loaded() { return isLoaded; }

	~Model();

	std::vector<Mesh*> meshList;

private:

	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);

	
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
	std::string m_name; // Used to refer to textures in case obj is in another folder
	std::string modelPath;

	bool isLoaded;
	int nInstances;

	//Default texture
	const char* PLAIN_PATH = "../../../Assets/plain.png"; //Path for default imag

	//EXTRA: Material custom a�adido
	Material materialModel;
};

