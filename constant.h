//
// Created by ColorsWind on 2021/10/17.
//

#ifndef CG_LAB2_2_CONSTANT_H
#define CG_LAB2_2_CONSTANT_H

#include "model.h"
#include <filesystem>
//#define _debug

using std::filesystem::path;

#ifdef _debug
const path current = path(__FILE__).parent_path() / "assets" ; // NOLINT
#else
const path current = std::filesystem::current_path() / "assets"; /* LINT */
#endif
// BEGINNOLINT
const path chairModel1 = current / "chairs/1be0108997e6aba5349bb1cbbf9a4206/models/model_normalized.obj";
const path chairModel2 = current / "chairs/2f1bc92c53f359d759a6208793b9dfca/models/model_normalized.obj";
const path chairModel3 = current / "chairs/5cdf1e5cc2fdacedb01116e8ff585621/models/model_normalized.obj";
const path chairModel4 = current / "hairs/5f5a3d412807984a27f9863535eba56c/models/model_normalized.obj";
const path chairModel5 = current / "chairs/9ab4d3826bd2f07b51bb569b4bf0aada/models/model_normalized.obj";
const path tableModel1  = current / "tables/1a8a796da899cb2c4d672fe014b9000e/models/model_normalized.obj";
const path tableModel2  = current / "tables/1a8fe5baa2d4b5f7ee84261b3d20656/models/model_normalized.obj";
const path tableModel3  = current / "tables/1a9bb6a5cf448d75e6fd255f2d77a585/models/model_normalized.obj";
const path tableModel4  = current / "tables/1a9ea91307d15d91f51f77a6d7299806/models/model_normalized.obj";
const path tableModel5  = current / "tables/1a767b8130e8a5722a7d46e74f08da70/models/model_normalized.obj";
const path MAPS_VERTEX_SHADER = "lighting_maps.vs";
const path MAPS_FRAGMENT_SHADER = "lighting_maps.fs";
// ENDNOLINT

namespace ModelNormal {
    const glm::vec3 up    = {0.0F, 1.0F, 0.0F};
    const glm::vec3 right = {1.0F, 0.0F, 0.0F};
    const glm::vec3 front = {0.0F, 0.0F, -1.0F};
    const glm::vec3 down  = -up;
    const glm::vec3 left  = -right;
    const glm::vec3 back  = -front;

    const glm::vec2 texCoords[] = {
            {0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}, {0.0F, 1.0F}
    };

}
namespace RoomModel {
    using namespace ModelNormal;
    const path texture0 = current / "tables/1a8a796da899cb2c4d672fe014b9000e/images/texture0.jpg";
    const path texture1 = current / "tables/1a8a796da899cb2c4d672fe014b9000e/images/texture1.jpg";

    const glm::vec3 vertices[8] = {
            {0.0F,   0.0F,   0.0F},
            {100.0F, 0.0F,   0.0F},
            {100.0F, 0.0F,   60.0F},
            {0.0F,   0.0F,   60.0F},
            {0.0F,   30.0F,  0.0F},
            {100.0F, 30.0F,  0.0F},
            {100.0F, 30.0F,  60.0F},
            {0.0F,   30.0F,  60.0F},
    };



    const Vertex surfaces[30]= {
            // 底面 0 1 2 3
            {vertices[0], up, texCoords[0]},
            {vertices[2], up, texCoords[2]},
            {vertices[3], up, texCoords[3]},
            {vertices[2], up, texCoords[2]},
            {vertices[0], up, texCoords[0]},
            {vertices[1], up, texCoords[1]},

            // 左面 0 3 4 7
            {vertices[0], right, texCoords[0]},
            {vertices[7], right, texCoords[2]},
            {vertices[4], right, texCoords[3]},
            {vertices[7], right, texCoords[2]},
            {vertices[0], right, texCoords[0]},
            {vertices[3], right, texCoords[1]},

            // 右面 1 2 5 6
            {vertices[1], left, texCoords[0]},
            {vertices[6], left, texCoords[2]},
            {vertices[2], left, texCoords[3]},
            {vertices[6], left, texCoords[2]},
            {vertices[1], left, texCoords[0]},
            {vertices[5], left, texCoords[1]},

            // 前面 2 3 6 7
            {vertices[2], back, texCoords[0]},
            {vertices[7], back, texCoords[2]},
            {vertices[3], back, texCoords[3]},
            {vertices[7], back, texCoords[2]},
            {vertices[2], back, texCoords[0]},
            {vertices[6], back, texCoords[1]},

            // 后面 0 1 4 5
            {vertices[0], front, texCoords[0]},
            {vertices[5], front, texCoords[2]},
            {vertices[1], front, texCoords[3]},
            {vertices[5], front, texCoords[2]},
            {vertices[0], front, texCoords[0]},
            {vertices[4], front, texCoords[1]},
    };
}

namespace LightModel {
    using namespace ModelNormal;
    const glm::vec3 vertices[8] = {
            {0.0F, 0.0F, 0.0F},
            {1.0F, 0.0F, 0.0F},
            {1.0F, 0.0F, 1.0F},
            {0.0F, 0.0F, 1.0F},
            {0.0F, 1.0F, 0.0F},
            {1.0F, 1.0F, 0.0F},
            {1.0F, 1.0F, 1.0F},
            {0.0F, 1.0F, 1.0F},
    };

    const Vertex surfaces[36]= {

            // 左面 0 3 4 7
            {vertices[0], left, texCoords[0]},
            {vertices[7], left, texCoords[2]},
            {vertices[4], left, texCoords[3]},
            {vertices[7], left, texCoords[2]},
            {vertices[0], left, texCoords[0]},
            {vertices[3], left, texCoords[1]},

            // 右面 1 2 5 6
            {vertices[1], right, texCoords[0]},
            {vertices[6], right, texCoords[2]},
            {vertices[2], right, texCoords[3]},
            {vertices[6], right, texCoords[2]},
            {vertices[1], right, texCoords[0]},
            {vertices[5], right, texCoords[1]},

            // 前面 2 3 6 7
            {vertices[2], front, texCoords[0]},
            {vertices[7], front, texCoords[2]},
            {vertices[3], front, texCoords[3]},
            {vertices[7], front, texCoords[2]},
            {vertices[2], front, texCoords[0]},
            {vertices[6], front, texCoords[1]},

            // 后面 0 1 4 5
            {vertices[0], back, texCoords[0]},
            {vertices[5], back, texCoords[2]},
            {vertices[1], back, texCoords[3]},
            {vertices[5], back, texCoords[2]},
            {vertices[0], back, texCoords[0]},
            {vertices[4], back, texCoords[1]},

            // 底面 0 1 2 3
            {vertices[0], down, texCoords[0]},
            {vertices[2], down, texCoords[2]},
            {vertices[3], down, texCoords[3]},
            {vertices[2], down, texCoords[2]},
            {vertices[0], down, texCoords[0]},
            {vertices[1], down, texCoords[1]},

            // 顶面 0 1 2 3
            {vertices[0], up, texCoords[0]},
            {vertices[2], up, texCoords[2]},
            {vertices[3], up, texCoords[3]},
            {vertices[2], up, texCoords[2]},
            {vertices[0], up, texCoords[0]},
            {vertices[1], up, texCoords[1]},
    };
}
GLfloat velocity = 25.0F;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;


#endif //CG_LAB2_2_CONSTANT_H
