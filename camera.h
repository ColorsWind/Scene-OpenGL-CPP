//
// Created by ColorsWind on 2021/10/21.
//
#ifdef _WIN32
// 保证 Windows 系统能正确显示中文
#pragma execution_character_set("utf-8")
#endif
#ifndef CG_LAB2_CAMERA_H
#define CG_LAB2_CAMERA_H

#include <sstream>
#include <iomanip>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "constant.h"
#include "shader.h"

class Camera {
private:
    const glm::vec3 cameraUp = {0.0F, 1.0F, 0.0F};

    glm::vec3 cameraPosition = glm::vec3(30.0F, 10.0F, 50.0F);

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    GLfloat lastX = 0, lastY = 0;
    GLfloat yaw = 0, pitch = 0;
    GLfloat zoom = 90.0F;
    GLdouble lastFrame;
    GLdouble lastFrameFPS = glfwGetTime();
    GLdouble fps = 0.0;
    bool caught;
    char windowTitle[1024];
    int scanWidth = SCR_WIDTH;
    int scanHeight = SCR_HEIGHT;


    void updateView() {
        front = glm::normalize(glm::vec3{
                cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        });
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, cameraUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, cameraUp));
    }

    void updateTitle(GLFWwindow *window) {
        std::stringstream ss;
        using std::fixed, std::setprecision;
        ss << "OpenGL 3D场景渲染    "
           << "x: " << fixed << setprecision(1) << cameraPosition.x
           << " y: " << fixed << setprecision(1) << cameraPosition.y
           << " z: " << fixed << setprecision(1) << cameraPosition.z
           << " yaw: " << fixed << setprecision(2) << yaw
           << " pitch: " << fixed << setprecision(1) << pitch;
        ss.getline(windowTitle, sizeof(windowTitle) / sizeof(char));
        glfwSetWindowTitle(window, windowTitle);
    }

public:
    Camera() : lastFrame(-1.0F), caught(false) {

    }

    void setShader(Shader *shader) {
        updateView();
        // View Transformation
        glm::mat4 viewTrans = glm::lookAt(cameraPosition, cameraPosition + front, cameraUp);
        shader->setMat4("view", viewTrans);

        // Projection Transformation
        glm::mat4 projTrans = glm::perspective(glm::radians(zoom), float(scanWidth) / float(scanHeight), 1.0F, 150.0F);
        shader->setMat4("projection", projTrans);

    }

    void onKeyboard(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            // 关闭窗口
            glfwSetWindowShouldClose(window, true);
            return;
        }
        GLdouble currentFrame = glfwGetTime();
        GLdouble deltaFrame = 0.1F;
        if (lastFrame > 0) {
            deltaFrame = currentFrame - lastFrame;
        }
        lastFrame = currentFrame;
        auto step = (GLfloat)(velocity * deltaFrame);
        bool fallback = true;
        glm::vec3 movement = {0.0F, 0.0F, 0.0F};
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // 向前
            movement += front * step * glm::vec3(1,0,1);
            fallback = false;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // 向后
            movement -= front * step * glm::vec3(1,0,1);
            fallback = false;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // 向左
            movement -= right * step * glm::vec3(1,0,1);
            fallback = false;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // 向右
            movement += right * step * glm::vec3(1,0,1);
            fallback = false;
        }
        float length = glm::length(movement);
        if (length > 0) {
            movement = movement * step / length;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            // 向上
            movement.y += step;
            fallback = false;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            // 向下
            movement.y -= step;
            fallback = false;
        }
        if (fallback) {
            lastFrame = -1;
            return;
        }
        cameraPosition += movement;

        updateView();
        updateTitle(window);
    }

    void onMouseMove(GLFWwindow *window, double x, double y) {
        if (!caught) return;
        if (lastX < 0 && lastY < 0) {
            lastX = (GLfloat)x;
            lastY = (GLfloat)y;
        }

        float offsetX = (GLfloat)x - lastX;
        float offsetY = lastY - (GLfloat)y;
        lastX = (GLfloat)x;
        lastY = (GLfloat)y;

        float sensitivity = 0.05;
        offsetX *= sensitivity;
        offsetY *= sensitivity;

        yaw += offsetX;
        pitch += offsetY;

        pitch = fmax(pitch, -89.0F);
        pitch = fmin(pitch, 89.0F);

        yaw = fmodf(yaw, 360.0F);
        updateTitle(window);
    }

    void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
        if (action != GLFW_PRESS) return;
        if (button != GLFW_MOUSE_BUTTON_LEFT && button != GLFW_MOUSE_BUTTON_RIGHT) return;
        caught = !caught;
        if (caught) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            lastX = (float)x;
            lastY = (float)y;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    void onSizeChanged(GLFWwindow *window, int width, int height) {
        this->scanWidth = width;
        this->scanHeight = height;
        glViewport(0, 0, width, height);
    }


    void onScrolling(GLFWwindow *window, double xOffset, double yOffset) {
        zoom -= (GLfloat)yOffset;
        zoom = fmax(45.0F, fmin(zoom, 150.0F));
    }

    glm::vec3 &getCameraPosition() {
        return cameraPosition;
    }



    void updateFps() {
        GLdouble currentFrameFPS = glfwGetTime();
        std::swap(lastFrame, currentFrameFPS);
        fps = 1 / (lastFrame - currentFrameFPS);
    }


};
#endif //CG_LAB2_CAMERA_H
