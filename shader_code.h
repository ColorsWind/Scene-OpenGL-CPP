//
// Created by ColorsWind on 2021/10/23.
//

#ifndef CG_LAB2_SHADER_CODE_H
const char* verticesShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;      // 位置坐标
layout (location = 1) in vec3 aNormal;     // 法向坐标
layout (location = 2) in vec2 aTexCoord; // 纹理坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;
void main()
{
   Normal = mat3(transpose(inverse(model))) * aNormal;
   FragPos = vec3(model * vec4(aPos, 1.0));
   TexCoord = aTexCoord;

   gl_Position = projection * view * model * vec4(aPos, 1.0F);
}

)";
const char* fragmentShader = R"(#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture1;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool lightObject;

void main()
{
    if (lightObject) {
        FragColor = vec4(1.0F);
    } else {
        // ambient
        vec3 ambient = light.ambient * (texture(texture1, TexCoord).rgb + material.diffuse);

        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * (texture(texture1, TexCoord).rgb + material.diffuse);

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * material.specular;

        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);

    }

})";


#define CG_LAB2_SHADER_CODE_H

#endif //CG_LAB2_SHADER_CODE_H
