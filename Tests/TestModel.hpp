#ifndef TEST_MODELS_H
#define TEST_MODELS_H
#include "Utility.hpp"

#include "../src/Graficos/Modelo/Model.h"

int testModels()
{
	int fails = 0;
	Model m(0.9f, 1.0f, "../../../Assets/Moneda/Moneda.obj", "Moneda");
	m.Load();

	// Mesh
	fails += Assert_bool(m.meshList[0]->GetVertices()->size() == 1760, "Model Vertices");

	// Texture
	fails += Assert_bool(m.textureList[0]->getWidth() == 64, "Model Texutre: Width");
	fails += Assert_bool(m.textureList[0]->getHeight() == 64, "Model Texutre: Height");
	fails += Assert_bool(m.textureList[0]->getbitDepth() == 4, "Model Texutre: Bit Depth");

	// Material
	fails += Assert_bool(m.getMaterial().getShininess() == 1.0f, "Model Material: Shininess");
	fails += Assert_bool(m.getMaterial().getSpecIntensity() == 0.9f, "Model Material: Specular Intensity");

	//Nom
	fails += Assert_bool(m.GetName() == "Moneda", "Model Name");
	
	// Add Instance
	m.setInstances(99);
	fails += Assert_bool(m.getNInstances() == 99, "Model Instances");

	return fails;
}
#endif