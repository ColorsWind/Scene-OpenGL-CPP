//
// Created by ColorsWind on 2021/10/21.
//

#ifndef CG_LAB2_LOADER_H
#include <memory>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>


// v vt vn 的索引
class IndicesVTN {
public:
    unsigned v;
    unsigned vt;
    unsigned vn;

    IndicesVTN(unsigned int v, unsigned int vt, unsigned int vn);

    explicit IndicesVTN(std::string &str);

private:

    static inline bool isDigit(std::string &str);

    inline void parseFaceComponent(std::string &str);
};

class Material;
struct Vertex;
class ObjModel;
struct SubModel;
// .obj模型加载器
class ObjModelLoader {
    friend class ObjModel;
private:
    std::vector<glm::vec3> geometricVertices;
    std::vector<glm::vec2> textureVertices;
    std::vector<glm::vec3> vertexNormals;



    void inline appendSurfaceVertices(const IndicesVTN &indices, std::vector<Vertex> &surfaceVertices);

    void inline appendSurfaceVertices(std::stringstream &ss, std::vector<Vertex> &surfaceVertices);

    static void inline inputVec3(std::stringstream &ss, glm::vec3 &vec3);

    static void inline appendVec2(std::stringstream &ss, std::vector<glm::vec2> &vectors);

    static void inline appendVec3(std::stringstream &ss, std::vector<glm::vec3> &vectors);

    static inline bool ignore(const std::string &str);

public:
    ObjModelLoader() {
        // OBJ模型的索引从1开始, 我们将第0个元素作为找不到元素的默认值
        geometricVertices.emplace_back(0.0F, 0.0F, 0.0F);
        textureVertices.emplace_back(0.0F, 0.0F);
        vertexNormals.emplace_back(0.0F, 0.0F, 0.0F);
    }

    void loadMTL(const std::filesystem::path &path, ObjModel *parent);

    void loadObj(const std::filesystem::path &path, ObjModel *parent);

};
#define CG_LAB2_LOADER_H

#endif //CG_LAB2_LOADER_H
