#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 vertexColor;
out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;
out vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

void main()
{
    vertexColor = aColor;
    TexCoord = aTexCoord;
    normal = mat3(transpose(inverse(view * model))) * aNormal;
    fragPos = vec3(view * model * vec4(aPos, 1.0));
    lightPos = vec3(view * vec4(lightPosition, 1.0));
    gl_Position = projection * vec4(fragPos, 1.0);
}