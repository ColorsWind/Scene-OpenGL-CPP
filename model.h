//
// Created by ColorsWind on 2021/10/19.
//

#ifndef CG_LAB2_MODEL_H
#define CG_LAB2_MODEL_H

#include <utility>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <fstream>
#include <sstream>

#include "shader.h"
#include "loader.h"
typedef unsigned int model_id_t;
// 代表纹理贴图
class Texture {
    friend class Material;
public:
    // 调用一次send之后才会被填充
    int width = -1, height = -1, nrChannels = -1;
private:

    model_id_t id = 0;
    // 调用一次send之后才会被填充
    std::filesystem::path path;

    Texture(std::filesystem::path path) : path(std::move(path)) {}

public:
    void send();

    void use() {
        assert(id != 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
    }


    static Texture* createEmptyTexture() {
        auto *texture = new Texture("");
        glGenTextures(1, &texture->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glGenerateMipmap(GL_TEXTURE_2D);
        return texture;
    }


};

// 代表材质
class Material {
public:
    glm::vec3 ambient = {0.0F, 0.0F, 0.0F};    // 环境光下的颜色
    glm::vec3 diffuse = {0.0F, 0.0F, 0.0F};    // 漫反射下的颜色
    glm::vec3 specular = {0.0F, 0.0F, 0.0F};   // 镜面反射的影响
    float shininess = 32.0F;      // 镜面高光半径
    Texture *texture = nullptr;

    Material() {}

//    Material(const Material &material) = delete;
//    Material(Material &&material) = default;

    void send() {
        if (texture)
            texture->send();
    }
    void createTexture(const std::filesystem::path &path) {
        this->texture = new Texture(path);
    }

    void use(Shader *shader) {
        if (texture) {
            texture->use();
        } else {
            static auto *emptyTexture = Texture::createEmptyTexture();
            emptyTexture->use();
//            glBindTexture(GL_TEXTURE_2D, NULL);
//            glActiveTexture(GL_TEXTURE0);
        }
        shader->setVec3("material.ambient", ambient);
        shader->setVec3("material.diffuse", diffuse);
        shader->setVec3("material.specular", specular);
        shader->setFloat("material.shininess", shininess);
    }




};

// 子模型，用同一种材质
struct SubModel {
    unsigned begin;
    unsigned length;
    Material *material;

    SubModel(unsigned begin, Material *material) : begin(begin), length(0), material(material) {
        assert(material);
    }

    SubModel(unsigned begin, unsigned length, Material *material) : begin(begin), length(length),
                                                                            material(material) {
        assert(material);
    }

    inline void draw(Shader *shader) {
        material->use(shader);
        glDrawArrays(GL_TRIANGLES, begin, length);
    }


};

// 顶点，会被发送到GPU
struct Vertex {
    glm::vec3 position; // 位置坐标
    glm::vec3 normal; // 法线坐标
    glm::vec2 texCoords; // 纹理坐标
};



class Scene;
class ObjModel;


class Scene {
    friend class ObjModel;
    friend class ObjModelLoader;
private:
    std::vector<Vertex> surfaceVertices;
    std::vector<ObjModel> models;
    unsigned int VBO, VAO;
public:
    void init();

    void use();

    void send();

    ObjModel &createModel(const std::string &name) ;

    ObjModel &copyModel(model_id_t id, const std::string &newName);

    void draw(Shader *shader);

    ObjModel & getModelById(model_id_t id);
};

class ObjModel {
    friend class ObjModelLoader;
    friend class Scene;
private:
    // Scene填充
    model_id_t id = -1;
    std::unordered_map<std::string, Material> materials;
    std::vector<SubModel> submodels;
    Scene *parent;
public:

    glm::mat4 modelTrans;
    bool lightCube = false;
    const std::string name;

    ObjModel(Scene *scene, std::string name) : parent(scene), modelTrans(glm::mat4(1.0F)), name(std::move(name)) {
        if (!scene)
            throw std::runtime_error("Scene is nullptr");
    };

    ObjModel(const ObjModel &objModel) = delete;
    ObjModel(ObjModel &&objModel) = default;

    unsigned getId() const {
        return id;
    }
    void loadFromFile(const std::string &path);

    void loadFromVertices(const Vertex *vertices, unsigned length, Material *material);

    void sendTexture() {
        for (auto iter=materials.begin();iter !=materials.cend();iter++) {
            iter->second.send();
        }
    }


    void draw(Shader *shader);

    void moveToFloor(GLfloat y) {
        GLuint begin = submodels.front().begin;
        GLuint end   = submodels.back().begin + submodels.back().length;
        GLfloat minY = 1.0E10;
        for(unsigned i=begin;i<end;i++) {
            glm::vec3 &pos = parent->surfaceVertices[i].position;
            minY = fmin(minY, (modelTrans * glm::vec4{pos.x, pos.y, pos.z, 1.0}).y);
        }
        modelTrans = glm::translate(modelTrans, {0.0, -minY + y, 0.0});
    }
};

#endif //CG_LAB2_MODEL_H
