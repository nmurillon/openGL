#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Shader.hpp>
#include <libs/renderer/Texture.hpp>
#include <libs/renderer/Vertex.hpp>
#include <libs/renderer/VertexArray.hpp>
#include <libs/renderer/VertexBuffer.hpp>

#include <vector>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT Mesh {
public:
public:
  Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned int> &&indices,
       std::vector<Texture> &&textures);

  Mesh(const Mesh &) = delete;
  Mesh(Mesh &&other) noexcept;
  Mesh &operator=(const Mesh &) = delete;
  Mesh &operator=(Mesh &&other);

  void draw(Shader &shader) const;

public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

private:
  void init();

  VertexArray m_vao;
};
} // namespace libs::renderer