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

