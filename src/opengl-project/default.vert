#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

uniform float scale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
   vec4 worldPosition = model * vec4(aPos, 1.0);
   gl_Position = proj * view * worldPosition;
   color = aColor;

   texCoord = aTex;
   FragPos = vec3(worldPosition);
   Normal = mat3(transpose(inverse(model))) * aNormal;
}
