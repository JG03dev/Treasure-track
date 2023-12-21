#ifndef TEST_RENDERER_H
#define TEST_RENDERER_H
#include "Utility.hpp"

#include "../src/Graficos/Renderer.h"

int testRenderer()
{
	int fails = 0;
	Renderer R("../../../Tests/testRenderer.json", 800, 600, 0, 0);

	fails += Assert_bool(R.getNDirLights() == 1, "Parser: NDirLights");
	fails += Assert_bool(R.getNPointLights() == 1, "Parser: NPointLights");
	fails += Assert_bool(R.getNSpotLights() == 1, "Parser: NSpotLights");

	fails += Assert_bool(R.getModelList().size() == 43, "Parser: NModels");

	fails += Assert_bool(R.getModel("Coin0i0j_1").first != nullptr, "Parser: Model Accessing");
	fails += Assert_bool(R.getModel("Coin0i0j_1").first->getNInstances() == 43, "Parser: Model Instances");

	fails += Assert_bool(R.getModel("Coin2i5j").second == glm::translate(glm::mat4(1.0f), glm::vec3(16.0, -3.4, 0.0)), "Parser: ModelMatrix");


	//Manually Adding Lights (we'll do it with pointers since Destructor handles the deallocation)
	DirectionalLight* dirLight = new DirectionalLight(1024, 1024, 1.0f, 1.0f, 1.0f, 0.5f, 0.8f, -1.0f, -1.0f, -1.0f, true);
	R.AddLight(dirLight);
	fails += Assert_bool(R.getNDirLights() == 2, "Adder: NDirLights");

	PointLight* pointLight = new PointLight(1024, 1024, 0.1f, 100.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.8f, 1.0f, 2.0f, 3.0f, 1.0f, 0.09f, 0.032f, true);
	R.AddLight(pointLight);
	fails += Assert_bool(R.getNPointLights() == 2, "Adder: NPointLights");

	SpotLight* spotLight = new SpotLight(1024, 1024, 0.1f, 100.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.8f,
		1.0f, 2.0f, 3.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.09f, 0.032f, 30.0f, true);
	R.AddLight(spotLight);
	fails += Assert_bool(R.getNSpotLights() == 2, "Adder: NSpotLights");

	//Adding a Model
	Model m(0.9f, 1.0f, "../../../Assets/Moneda/Moneda.obj", "Small model");
	// Add Instance
	m.setInstances(99);

	R.AddModel("TestModel", &m, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 2.0f)));

	fails += Assert_bool(R.getModelList().size() == 44, "Adder: NModels");	// Instances setted manually are not related with the ModelList size

	fails += Assert_bool(R.getModel("TestModel").first != nullptr, "Adder: Model Accessing");
	fails += Assert_bool(R.getModel("TestModel").first->getNInstances() == 99, "Adder: Model Instances");

	//Removing the model
	R.RemoveModel("TestModel");
	fails += Assert_bool(R.getModelList().size() == 43, "Remover: NModels");

	//SetModelMatrix
	R.setModelMatrix("Coin2i5j", glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 2.0f)));
	fails += Assert_bool(R.getModel("Coin2i5j").second == glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 2.0f)), "Setter: ModelMatrix");

	return fails;
}
#endif