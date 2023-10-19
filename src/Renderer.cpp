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
    model = glm::scale(model, glm::vec3(100, 1, 100));
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
    //
    // scale        
    // rotation     
    // translation  
    //
    

    /*
    meshList[0]->RenderMesh();

    // glm::mat4 model2;
    // btRigidBody* body = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[1]);
    // btTransform t;
    // body->getMotionState()->getWorldTransform(t);
    // t.getOpenGLMatrix(glm::value_ptr(model2));
    // model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv((int)uniformModel, 1, GL_FALSE, glm::value_ptr(model2));

    meshList[1]->RenderMesh();

    //model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    //glUniformMatrix4fv((int) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //
    //meshList[1]->RenderMesh();

    */

    glUseProgram(0);

    mainWindow->swapBuffers();
}