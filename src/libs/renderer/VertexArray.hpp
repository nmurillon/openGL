#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/IndexBuffer.hpp>
#include <libs/renderer/VertexBuffer.hpp>

#include <vector>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT VertexArray {
public:
  VertexArray();
  VertexArray(VertexBuffer &&vertexBuffer);

  VertexArray(const VertexArray &) = delete;
  VertexArray(VertexArray &&other) noexcept;
  VertexArray operator=(const VertexArray &) = delete;
  VertexArray &operator=(VertexArray &&other) noexcept;

  ~VertexArray();

  void setIndexBuffer(IndexBuffer &&indexBuffer);

  void addVertexBuffer(VertexBuffer &&vertexBuffer);

  int getVertexCount() const;

  void bind() const;
  static void unbind();

private:
  GLuint m_id;
  // Assume that the first buffer contains vertex data
  std::vector<VertexBuffer> m_buffers;
  IndexBuffer m_indexBuffer;

  unsigned int m_attributeOffset{0};
};
} // namespace libs::renderer