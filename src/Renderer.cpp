#include "Renderer.h"

void Renderer::render(btRigidBody* groundRB, btTransform chassisT) {
    shaderList[0]->UseShader();
    uniformModel = shaderList[0]->GetModelLocation();
    uniformProjection = shaderList[0]->GetProjectionLocation();
    uniformView = shaderList[0]->GetViewLocation();

    glm::mat4 model(1.0f);
    btVector3 scale = groundRB->getCollisionShape()->getLocalScaling();
    btTransform t = btTransform();
    groundRB->getMotionState()->getWorldTransform(t);
    t.getOpenGLMatrix(glm::value_ptr(model));
    model = glm::scale(model, glm::vec3(10, 1, 10));
    glUniformMatrix4fv((int)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv((int)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

    meshList[0]->RenderMesh();

    model = glm::mat4(1.0f);
    chassisT.getOpenGLMatrix(glm::value_ptr(model));
    model = glm::scale(model, glm::vec3(1.f, 0.5f, 2.f));
    glUniformMatrix4fv((int)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv((int)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

    meshList[0]->RenderMesh();

    glUseProgram(0);

    mainWindow->swapBuffers();
}