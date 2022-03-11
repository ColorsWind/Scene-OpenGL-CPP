#ifdef _WIN32
#pragma execution_character_set("utf-8")
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif
#include <iostream>
#include <iomanip>

#include <string>
#include "light.h"
#include "camera.h"

using std::fixed, std::setprecision, std::cout, std::cerr, std::endl;

Shader *shader;

Scene scene;
Camera *camera;
Light *light;


inline void initDrawing(GLFWwindow *window) {
    shader = new Shader(
            MAPS_VERTEX_SHADER.string(), MAPS_FRAGMENT_SHADER.string()
    );
    shader->use();
    camera = new Camera();
    scene.init();
    {
        auto & room = scene.createModel("房间");
        {
            auto *materialFloor = new Material();
            materialFloor->createTexture(RoomModel::texture1);
            materialFloor->send();
            materialFloor->shininess = 128.0F;
            room.loadFromVertices(RoomModel::surfaces, 6, materialFloor);
        }
        {
            auto *materialWall = new Material();
            materialWall->createTexture(RoomModel::texture0);
            materialWall->send();
            materialWall->shininess = 100.0F;
            room.loadFromVertices(RoomModel::surfaces + 6, 24, materialWall);
        }
    }
    {
        auto & chair = scene.createModel("电脑椅");
        // https://blog.csdn.net/zsq306650083/article/details/50561857/ 要求顺序：缩放 -> 旋转 -> 平移
        // 即 T * R * S * x 即 (((I * T) * R) * S) * x
        // 所以用glm的顺序为: 平移 -> 旋转 -> 缩放
        chair.loadFromFile(chairModel1.string());
        chair.modelTrans = glm::translate(chair.modelTrans, {20.0F, 7.0F, 40.0F});
        chair.modelTrans = glm::rotate(chair.modelTrans, glm::radians(27.0F), {0, 1, 0});
        chair.modelTrans = glm::scale(chair.modelTrans, {20.0F, 20.0F, 20.0F});
    }
    {
        auto & chair = scene.createModel("沙滩椅");
        chair.loadFromFile(chairModel2.string());
        chair.modelTrans = glm::translate(chair.modelTrans, {19.0F, 6.0F, 8.0F});
        chair.modelTrans = glm::rotate(chair.modelTrans, glm::radians(180.0F), {0, 1, 0});
        chair.modelTrans = glm::scale(chair.modelTrans, {20.0F, 20.0F, 20.0F});
    }
    {
        model_id_t chairs[6];
        {
            auto & chair = scene.createModel("普通椅子 #0");
            chair.loadFromFile(chairModel5.string());
            chairs[0] = chair.getId();
        }

        for(int i=1;i<6;i++)
            chairs[i] = scene.copyModel(chairs[0], "普通椅子 #" + std::to_string(i)).getId();

        for(int i=0;i<3;i++) {
            auto & chair = scene.getModelById(chairs[i]);
            chair.modelTrans = glm::translate(chair.modelTrans, {70.0F - abs(i - 1), 6.2F, 34.0F + i * 7.5F});
            chair.modelTrans = glm::rotate(chair.modelTrans, glm::radians(93.0F - 3 * i), {0, 1, 0});
            chair.modelTrans = glm::scale(chair.modelTrans, {15.0F, 15.0F, 15.0F});
        }
        for(int i=3;i<6;i++) {
            auto & chair = scene.getModelById(chairs[i]);
            chair.modelTrans = glm::translate(chair.modelTrans, {48.5F + abs(i -3 - 1), 6.2F, 34.0F + (i-3) * 7.5F});
            chair.modelTrans = glm::rotate(chair.modelTrans, glm::radians(267.0F + 3 * i), {0, 1, 0});
            chair.modelTrans = glm::scale(chair.modelTrans, {15.0F, 15.0F, 15.0F});
        }
    }
    {
        auto & table = scene.createModel("吧台");
        table.loadFromFile(tableModel2.string());
        table.modelTrans = glm::translate(table.modelTrans, {22.0F, 7.5F, 19.0F});
        table.modelTrans = glm::scale(table.modelTrans, {30.0F, 30.0F, 30.0F});
    }
    {
        auto & table = scene.createModel("中央桌子");
        table.loadFromFile(tableModel3.string());
        table.modelTrans = glm::translate(table.modelTrans, {60.0F, 5.5F, 42.0F});
        table.modelTrans = glm::rotate(table.modelTrans, glm::radians(90.0F), {0, 1, 0});
        table.modelTrans = glm::scale(table.modelTrans, {30.0F, 30.0F, 30.0F});
    }
    {
        auto & table = scene.createModel("柜子");
        table.loadFromFile(tableModel5.string());
        table.modelTrans = glm::translate(table.modelTrans, {95.0F, 7.1F, 8.0F});
        table.modelTrans = glm::rotate(table.modelTrans, glm::radians(90.0F), {0, 1, 0});
        table.modelTrans = glm::scale(table.modelTrans, {20.0F, 20.0F, 20.0F});
    }
    {
        light = new Light(&scene);
    }
    scene.send();
}

inline void onDraw(GLFWwindow *window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->setShader(shader);
    light->setShaderForLightedObjects(shader, camera->getCameraPosition());
    scene.draw(shader);
    glfwSwapBuffers(window);
}

void onSizeChanged(GLFWwindow *window, int width, int height) {
    camera->onSizeChanged(window, width, height);
}


void onMouseMove(GLFWwindow *window, double x, double y) {
    camera->onMouseMove(window, x, y);
}
void onMouseButton(GLFWwindow *window, int button, int action, int mods) {
    camera->onMouseButton(window, button, action, mods);
    light->onMouseButton(window, button, action, mods);
}

void onScrolling(GLFWwindow *window, double xOffset, double yOffset) {
    camera->onScrolling(window, xOffset, yOffset);
}

void processInput(GLFWwindow *window) {
    camera->onKeyboard(window);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL 3D场景渲染", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onSizeChanged);
    glfwSetScrollCallback(window, onScrolling);
    glfwSetCursorPosCallback(window, onMouseMove);
    glfwSetMouseButtonCallback(window, onMouseButton);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    initDrawing(window);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        onDraw(window);
        glfwPollEvents();

    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}