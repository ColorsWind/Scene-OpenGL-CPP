//
// Created by ColorsWind on 2021/10/19.
//

#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#ifdef _WIN32
  #define STBI_WINDOWS_UTF8
#endif
#include <stb_image.h>

#include <utility>


void Scene::init() {
    // 顶点数组对象
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 顶点缓冲对象
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 设置GLSL解释方式
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Scene::use() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}


void Scene::send() {
    // 发送顶点数据
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * surfaceVertices.size(),
                 surfaceVertices.data(),
                 GL_STATIC_DRAW);
    // 发送贴图
    for (auto iter = models.begin(); iter != models.cend(); iter++) {
        iter->sendTexture();
    }
}

void Scene::draw(Shader *shader) {
    for(auto iter = models.begin(); iter != models.cend(); iter++) {
        iter->draw(shader);
    }
}

ObjModel &Scene::createModel(const std::string &name) {
    auto & model = models.emplace_back(this, name);
    model.id = models.size() - 1;
    return model;
}

ObjModel & Scene::getModelById(model_id_t id) {
    return models[id];
}

ObjModel &Scene::copyModel(model_id_t id, const std::string &newName) {
    auto & objModel = models.emplace_back(this, newName);
    auto & oldModel = models[id];
    objModel.submodels.assign(oldModel.submodels.begin(), oldModel.submodels.end());
    objModel.id = models.size() - 1;
    return objModel;
}

void ObjModel::loadFromFile(const std::string &path) {
    ObjModelLoader loader;
    loader.loadObj(path, this);
}

void ObjModel::loadFromVertices(const Vertex *vertices, unsigned length, Material *material) {
    this->submodels.emplace_back((unsigned)parent->surfaceVertices.size(), length, material);
    parent->surfaceVertices.insert(parent->surfaceVertices.end(), vertices, vertices + length);

}

void ObjModel::draw(Shader *shader) {
    shader->setMat4("model", modelTrans);
    if (lightCube) {
        shader->setBool("lightObject", true);
        submodels.front().draw(shader);
        shader->setBool("lightObject", false);
    } else {
        for(auto iter=submodels.begin(); iter != submodels.cend(); iter++) {
            iter->draw(shader);
        }
    }

}

void Texture::send() {
    glGenTextures(1, &id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    if (!std::filesystem::exists(path)) {
        std::cerr << "找不到文件: " << path << std::endl;
    }

//    stbi_set_flip_vertically_on_load(true);
#ifdef _WIN32
    char buffer[1024];
    stbi_convert_wchar_to_utf8(buffer, sizeof(buffer), path.c_str());

    auto *data = stbi_load(buffer, &width, &height, &nrChannels, 0);
#else
    auto *data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
#endif

    std::wcout << path.c_str()  << "\twidth:" << width << "\theight: " << height << std::endl;
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        throw std::runtime_error("Only support 3 or 4 channels image.");
    }
    //stbi_write_jpg("test.png", width, height, nrChannels, data, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}
