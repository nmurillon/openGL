#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

layout(std140) uniform Matrices {
  mat4 model;
  mat4 view;
  mat4 projection;
};

out vec3 normal;

void main() {
  mat3 normalMat = mat3(transpose(inverse(view * model)));
  normal = normalize(vec3(vec4(normalMat * aNormal, 0.0)));
  gl_Position = view * model * vec4(aPos, 1.0);
}