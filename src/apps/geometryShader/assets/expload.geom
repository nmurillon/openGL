#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texCoords[];

out vec2 textCoords;

uniform float iTime;

float magnitude = 2.0;

vec3 getNormal() {
  vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
  vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

  return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
  vec3 direction = normal * magnitude * ((sin(iTime) + 1.0) / 2.0);
  return position + vec4(direction, 0.0);
}

void main() {
  vec3 normal = getNormal();
  gl_Position = explode(gl_in[0].gl_Position, normal);
  textCoords = texCoords[0];
  EmitVertex();

  gl_Position = explode(gl_in[1].gl_Position, normal);
  textCoords = texCoords[1];
  EmitVertex();

  gl_Position = explode(gl_in[2].gl_Position, normal);
  textCoords = texCoords[2];
  EmitVertex();

  EndPrimitive();
}