//
// Created by ColorsWind on 2021/10/21.
//
#include "loader.h"
#include "model.h"
void inline ObjModelLoader::appendSurfaceVertices(const IndicesVTN &indices, std::vector<Vertex> &surfaceVertices) {
    surfaceVertices.push_back({geometricVertices[indices.v],
                               vertexNormals[indices.vn],
                               textureVertices[indices.vt]
                              });
}

void inline ObjModelLoader::appendSurfaceVertices(std::stringstream &ss, std::vector<Vertex> &surfaceVertices) {
    std::string component;
    for (int k = 0; k < 3; k++) {
        ss >> component;
        IndicesVTN indices{component};
        appendSurfaceVertices(indices, surfaceVertices);
    }
}

void inline ObjModelLoader::inputVec3(std::stringstream &ss, glm::vec3 &vec3) {
    ss >> vec3.x >> vec3.y >> vec3.z;
}

void inline ObjModelLoader::appendVec2(std::stringstream &ss, std::vector<glm::vec2> &vectors) {
    glm::vec2 vec2;
    ss >> vec2.x >> vec2.y;
    vectors.push_back(vec2);
}

void inline ObjModelLoader::appendVec3(std::stringstream &ss, std::vector<glm::vec3> &vectors) {
    glm::vec3 vec3;
    inputVec3(ss, vec3);
    vectors.push_back(vec3);
}

bool ObjModelLoader::ignore(const std::string &str) {
    return str.empty() || str.front() == '#';
}

void ObjModelLoader::loadMTL(const std::filesystem::path &path, ObjModel *parent) {
    std::ifstream handle;
    handle.open(path);
    std::string buffer;
    std::string command;
    Material *currentMaterial;
    while (std::getline(handle, buffer)) {
        if (ignore(buffer)) continue;
        std::stringstream ss{buffer};
        ss >> command;
        if (command == "newmtl") {
            std::string name;
            ss >> name;
            auto result = parent->materials.emplace(
                    std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple());
            assert(result.second);
            currentMaterial = &result.first->second;
        } else if (command == "Kd") {
            inputVec3(ss, currentMaterial->diffuse);
        } else if (command == "Ka") {
            inputVec3(ss, currentMaterial->ambient);
        } else if (command == "Ks") {
            inputVec3(ss, currentMaterial->specular);
        } else if (command == "map_Kd") {
            std::string relativePath;
            ss >> relativePath;
            currentMaterial->createTexture(path.parent_path() / relativePath);
            currentMaterial->send();
        } else {
            // ignore illum, Ns, etc.
        }
    }
}

void ObjModelLoader::loadObj(const std::filesystem::path &path, ObjModel *parent) {
    std::ifstream handle;
    std::string buffer, command;
    if (!exists(path))
        throw std::runtime_error("找不到文件: " + path.string());
    handle.open(path);
    SubModel *context = nullptr;
    while (std::getline(handle, buffer)) {
        if (ignore(buffer)) continue;
        std::stringstream ss{buffer};
        ss >> command;
        if (command == "mtllib") {
            std::string relativePath;
            ss >> relativePath;
            loadMTL(path.parent_path() / relativePath, parent);
        } else if (command == "v") {
            appendVec3(ss, geometricVertices);
        } else if (command == "vt") {
            appendVec2(ss, textureVertices);
        } else if (command == "vn") {
            appendVec3(ss, vertexNormals);
        } else if (command == "f") {
            appendSurfaceVertices(ss, parent->parent->surfaceVertices);
            assert(context);
            context->length += 3;
        } else if (command == "usemtl") {
            // usemtl 命令
            // 设置上下文
            std::string currentMTL;
            ss >> currentMTL;
            auto iter = parent->materials.find(currentMTL);
            if (iter == parent->materials.cend())
                throw std::runtime_error("CANNOT find material: "  + currentMTL);
            context = &parent->submodels.emplace_back(
                    (unsigned)parent->parent->surfaceVertices.size(),
                    &iter->second
                );

        }
    }
}

IndicesVTN::IndicesVTN(unsigned int v, unsigned int vt, unsigned int vn) : v(v), vt(vt), vn(vn) {}

IndicesVTN::IndicesVTN(std::string &str) : v(0), vt(0), vn(0) {
    parseFaceComponent(str);
}

bool IndicesVTN::isDigit(std::string &str) {
    return std::all_of(str.cbegin(), str.cend(), [](auto c) { return std::isdigit(c); });
}

// Lines defining a face are started with "f". Faces are a set of three or more vertices,
// where each vertex follows the format "Position/Texture Coordinate/Normal". Each vertex
// is separated by a space. Each vertex defined in a face can contain a position and/or a
// texture coordinate, and/or a normal, where each number is separated by a "/".
// If a vertex contains only a position, there is no "/",
// if it contains a position and a texture coordinate, there is only one "/", which is between the two number.
// If the vertex contains a position, texture coordinate and a normal, there is a "/" between all three numbers.
// And last, if it contains only a position and a normal, there is a "//", or TWO "/" between the numbers.
void IndicesVTN::parseFaceComponent(std::string &str) {
    if (isDigit(str)) {
        this->v = std::stoi(str);
    } else if (std::count(str.cbegin(), str.cend(), '/') == 1) {
        GLuint index = str.find('/');
        this->v = std::stoi(str.substr(0, index));
        this->vt = std::stoi(str.substr(index + 1, str.length()));
    } else {
        unsigned first = str.find('/');
        unsigned second = str.find('/', first + 1);
        if (first + 1 == second) {
            this->v = std::stoi(str.substr(0, first));
            this->vn = std::stoi(str.substr(second + 1, str.length()));
        } else {
            this->v = std::stoi(str.substr(0, first));
            this->vt = std::stoi(str.substr(first + 1, second));
            this->vn = std::stoi(str.substr(second + 1, str.length()));
        }
    }
}
