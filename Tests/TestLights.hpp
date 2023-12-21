#ifndef TEST_LIGHTS_H
#define TEST_LIGHTS_H
#include "Utility.hpp"

#include "../src/Graficos/Luces/DirectionalLight.h"
#include "../src/Graficos/Luces/PointLight.h"
#include "../src/Graficos/Luces/SpotLight.h"

int testLights()
{
	int fails = 0;
	DirectionalLight dirLight(1024, 1024, 1.0f, 1.0f, 1.0f, 0.5f, 0.8f, -1.0f, -1.0f, -1.0f, true);

	// Check if values are initialized correctly
	glm::mat4 res(0.0353553407f, -0.0204124134f, -0.0115585634f, 0.0f,
		0.0f, 0.0408248268f, -0.0115585634f, 0.0f,
		-0.0353553407f, -0.0204124134f, -0.0115585634f, 0.0f,
		-0.0f, -0.0f, -0.967326283f, 1.0f);
	fails += Assert_bool(dirLight.CalculateLightTransform() == res, "Directional Constructor: CLT");
	fails += Assert_bool(dirLight.getShadowMap()->GetShadowHeight() == 1024 && dirLight.getShadowMap()->GetShadowWidth() == 1024, "Directional Constructor: Shadow of Directional");

	PointLight pointLight(1024, 1024, 0.1f, 100.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.8f, 1.0f, 2.0f, 3.0f, 1.0f, 0.09f, 0.032f, true);

	fails += Assert_bool(pointLight.GetFarPlane() == 100.0f, "Point Constructor: Far plane");
	fails += Assert_bool(pointLight.GetPosition() == glm::vec3(1.0f, 2.0f, 3.0f), "Point Constructor: Position");
	fails += Assert_bool(pointLight.getShadowMap()->GetShadowHeight() == 1024 && dirLight.getShadowMap()->GetShadowWidth() == 1024, "Point Constructor: Shadow of Directional");


	SpotLight spotLight(1024, 1024, 0.1f, 100.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.8f,
		1.0f, 2.0f, 3.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.09f, 0.032f, 30.0f, true);

	fails += Assert_bool(spotLight.GetFarPlane() == 100.0f, "Spot Constructor: Far plane");
	fails += Assert_bool(spotLight.GetPosition() == glm::vec3(1.0f, 2.0f, 3.0f), "Spot Constructor: Position");
	fails += Assert_bool(spotLight.getShadowMap()->GetShadowHeight() == 1024 && dirLight.getShadowMap()->GetShadowWidth() == 1024, "Spot Constructor: Shadow of Directional");


	return fails;
}

#endif