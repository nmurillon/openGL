#version 330 core

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  float shininess;
};

in vec2 texCoords;

uniform Material material;

out vec4 FragColor;

void main() { FragColor = texture(material.texture_diffuse1, texCoords); }