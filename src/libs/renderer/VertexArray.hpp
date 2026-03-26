#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/IndexBuffer.hpp>
#include <libs/renderer/VertexBuffer.hpp>

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

  void setVertexBuffer(VertexBuffer &&vertexBuffer);
  void setIndexBuffer(IndexBuffer &&indexBuffer);

  void bind() const;
  static void unbind();

private:
  GLuint m_id;
  // TODO: we might have mltiple buffers in the future
  VertexBuffer m_vertexBuffer;
  IndexBuffer m_indexBuffer;
  int m_vertexCount{0};
};
} // namespace libs::renderer