#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>

#include <cstddef>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT IndexBuffer {
public:
  IndexBuffer();
  virtual ~IndexBuffer() = default;

  void setData(const void *data, std::size_t size,
               GLenum usage = GL_STATIC_DRAW);

  void bind() const;
  static void unbind();

private:
  GLuint m_id;
  const void *m_data;
};
} // namespace libs::renderer