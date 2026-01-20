#pragma once

#include <logl/renderer/export.h>

#include <vector>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT Cube {
public:
  enum class LOGL_RENDERER_EXPORT DataType {
    POSITION,
    POSITION_COLOR,
    POSITION_TEXTURE,
    POSITION_NORMAL,
    POSITION_NORMAL_TEXTURE,
  };

  Cube(const DataType &type, const std::vector<float> &vertices,
       const std::vector<unsigned int> &indices = {});

  void draw() const;

private:
  int getVertexOffset() const;
  int getStride() const;

private:
  DataType m_type;
  std::vector<float> m_vertices;
  std::vector<unsigned int> m_indices;

  unsigned int m_VAO;
  unsigned int m_VBO;
  unsigned int m_EBO;
};
} // namespace libs::renderer