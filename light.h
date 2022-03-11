//
// Created by ColorsWind on 2021/10/22.
//

#ifndef CG_LAB2_LIGHT_H
#define CG_LAB2_LIGHT_H
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "constant.h"
class Light {
private:
    glm::vec3 position = {10.0F, 30.0F, 10.0F};
    glm::vec3 scale    = {7.0F, 7.0F, 7.0F};
    ObjModel *objModel;
    bool lightMove = true;
public:
    Light(Scene *scene) {
        objModel = &scene->createModel("光源");
        objModel->loadFromVertices(LightModel::surfaces, sizeof(LightModel::surfaces) / sizeof(Vertex), new Material());
        objModel->lightCube = true;
        objModel->modelTrans = glm::translate(objModel->modelTrans, position);
        objModel->modelTrans = glm::scale(objModel->modelTrans, {10.0F, 10.0F, 10.0F});
    }

    inline GLdouble normalization(GLdouble sinLike) {
        return (sinLike + 1) / 2;
    }

    void updateLightPosition() {
        if (!lightMove) return;
        position.x = (float)normalization(sin(glfwGetTime() / 5 * 2.66)) * 140 - 20;
        position.y = (float)normalization(sin(glfwGetTime() / 7 * 2.33)) * 20 + 40;
        position.z = (float)normalization(sin(glfwGetTime() / 3 * 2.11)) * 80 - 10;
        objModel->modelTrans = glm::translate(glm::mat4(1.0F), position);
        objModel->modelTrans = glm::scale(objModel->modelTrans, scale);
    }

    void setShaderForLightedObjects(Shader *shader, glm::vec3 &cameraPosition) {
        // be sure to activate shader when setting uniforms/drawing objects
        shader->use();
        updateLightPosition();
        shader->setVec3("light.position", position);
        shader->setVec3("viewPos", cameraPosition);

        // light properties
        shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    }

    void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
        if (action != GLFW_PRESS) return;
        if (button != GLFW_MOUSE_BUTTON_MIDDLE) return;
        lightMove = !lightMove;
    }
};
#endif //CG_LAB2_LIGHT_H
