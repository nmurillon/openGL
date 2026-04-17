#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec3 normal[];

layout(std140) uniform Matrices {
  mat4 model;
  mat4 view;
  mat4 projection;
};

const float MAGNITUDE = 0.4;

void line(int index) {
  gl_Position = projection * gl_in[index].gl_Position;
  EmitVertex();

  gl_Position = projection * (gl_in[index].gl_Position +
                              vec4(normal[index], 0.0) * MAGNITUDE);
  EmitVertex();
  EndPrimitive();
}

void main() {
  line(0);
  line(1);
  line(2);
}