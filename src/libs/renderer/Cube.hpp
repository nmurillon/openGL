#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/BufferLayout.hpp>
#include <libs/renderer/VertexArray.hpp>

#include <vector>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT Cube {
public:
  Cube(BufferLayout &&layout, const std::vector<float> &vertices,
       const std::vector<unsigned int> &indices = {});

  void draw() const;

private:
  VertexArray m_vertexArray;

  std::vector<float> m_vertices{};
  std::vector<unsigned int> m_indices{};
};
} // namespace libs::renderer