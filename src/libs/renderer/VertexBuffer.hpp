#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>
#include <libs/renderer/BufferLayout.hpp>

#include <cstddef>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT VertexBuffer {
public:
  VertexBuffer();
  VertexBuffer(BufferLayout &&layout);
  VertexBuffer(BufferLayout &&layout, const void *data, std::size_t size,
               GLenum usage = GL_STATIC_DRAW);

  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer(VertexBuffer &&other) noexcept;
  VertexBuffer operator=(const VertexBuffer &) = delete;
  VertexBuffer &operator=(VertexBuffer &&other) noexcept;

  ~VertexBuffer();

  void bind() const;
  static void unbind();

  void setLayout(BufferLayout &&layout);
  const BufferLayout &getLayout() const;
  void setData(const void *data, std::size_t size,
               GLenum usage = GL_STATIC_DRAW);

private:
  GLuint m_id;
  BufferLayout m_layout;
  const void *m_data{nullptr};
};
} // namespace libs::renderer