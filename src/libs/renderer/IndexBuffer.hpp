#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>

#include <cstddef>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT IndexBuffer {
public:
  IndexBuffer();
  IndexBuffer(const void *data, std::size_t size,
              GLenum usage = GL_STATIC_DRAW);
  IndexBuffer(const IndexBuffer &) = delete;
  IndexBuffer(IndexBuffer &&other) noexcept;
  IndexBuffer operator=(const IndexBuffer &) = delete;
  IndexBuffer &operator=(IndexBuffer &&other) noexcept;
  ~IndexBuffer();

  void setData(const void *data, std::size_t size,
               GLenum usage = GL_STATIC_DRAW);

  void bind() const;
  static void unbind();

private:
  GLuint m_id;
  const void *m_data;
};
} // namespace libs::renderer