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
  // TODO: see to create buffer with data in the constructor
  virtual ~VertexBuffer() = default;

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