#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>
#include <libs/renderer/Texture.hpp>

namespace libs::renderer {

class LOGL_RENDERER_EXPORT FrameBuffer {
public:
  FrameBuffer(int width, int size);
  ~FrameBuffer();
  FrameBuffer(const FrameBuffer &) = delete;
  FrameBuffer(FrameBuffer &&);
  FrameBuffer &operator=(const FrameBuffer &) = delete;
  FrameBuffer &operator=(FrameBuffer &&);

  void bind() const;
  static void unbind();

  const Texture &getColorBuffer() const;
  void setSize(int width, int height);

private:
  GLuint m_id;
  Texture m_colorAttachment;

  // TODO: create a RenderBuffer class
  GLuint m_renderBufferId;
};
} // namespace libs::renderer