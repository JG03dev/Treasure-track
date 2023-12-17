#include "../src/Graficos/Renderer.h"
#include <iostream>
#include <cassert>



int main() {
    
    std::cout << "Running tests" << std::endl;

    std::cout << "Unit Tests" << std::endl;

    // Unit test go here

    std::cout << "Integration test" << std::endl;
    std::cout << "Running integration test..." << std::endl;
    Renderer renderer("Parser", 800, 600);

    // Test getters
    assert(renderer.getNDirLights() == 1);
    assert(renderer.getNPointLights() == 1);
    assert(renderer.getNSpotLights() == 1);

    // Test model-related functions
    assert(renderer.getModelList().size() == 1);
    assert(renderer.getModel("model_id").first == &model);

    // Test rendering functions
    std::cout << "Rendering everything...\n";
    renderer.RenderEverything(/* pass appropriate Camera and projectionMatrix */);

    std::cout << "Rendering shadow for directional light...\n";
    renderer.RenderShadowDirLight(&dirLight);

    std::cout << "Rendering objects...\n";
    renderer.RenderObjects(/* pass appropriate Camera and projectionMatrix */);

    std::cout << "Rendering scene...\n";
    renderer.RenderScene();

    // Additional tests for auxiliary functions can be added as needed

    std::cout << "All tests passed!\n";

    return 0;
}