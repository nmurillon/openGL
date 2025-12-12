#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Shader.hpp>
#include <libs/renderer/Texture.hpp>
#include <libs/renderer/Vertex.hpp>

#include <vector>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT Mesh {
public:
public:
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices,
       const std::vector<Texture> &textures);

  void draw(Shader &shader) const;

public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

private:
  void init();

  unsigned int m_vao;
  unsigned int m_vbo;
  unsigned int m_ebo;
};
} // namespace libs::renderer