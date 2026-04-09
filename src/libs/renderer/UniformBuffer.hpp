#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT UniformBuffer {
public:
  UniformBuffer(int size);

  void bind() const;
  static void unbind();

  void setBindingPoint(int bindingPoint) const;
  void setData(int offset, int size, const void *data) const;

private:
  GLuint m_id;
  int m_size;
};
} // namespace libs::renderer