#pragma once

#include <libs/openGl/opengl.h>

namespace libs::renderer {
class Buffer {
public:
  virtual ~Buffer() = default;
  virtual void bind() const = 0;
  virtual void unbind() const = 0;

protected:
  GLuint m_id;
};

} // namespace libs::renderer