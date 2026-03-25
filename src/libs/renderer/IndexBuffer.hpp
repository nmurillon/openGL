#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>
#include <libs/renderer/Buffer.hpp>

#include <cstddef>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT IndexBuffer : public Buffer {
public:
  IndexBuffer();
  virtual ~IndexBuffer() = default;

  void setData(const void *data, std::size_t size,
               GLenum usage = GL_STATIC_DRAW);

  virtual void bind() const override;
  virtual void unbind() const override;

private:
  const void *m_data;
};
} // namespace libs::renderer