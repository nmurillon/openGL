#include <libs/renderer/FrameBuffer.hpp>

#include <libs/logger/Logger.hpp>

namespace libs::renderer {
FrameBuffer::FrameBuffer(int width, int height)
    : m_colorAttachment(width, height) {
  glGenFramebuffers(1, &m_id);
  bind();

  // Attach the texture to the framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_colorAttachment.id(), 0);

  // Create a renderbuffer object for depth and stencil attachment
  glGenRenderbuffers(1, &m_renderBufferId);
  glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, m_renderBufferId);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    Logger::logError("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }

  unbind();
}

FrameBuffer::~FrameBuffer() {
  unbind();
  glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_id); }

void FrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

const Texture &FrameBuffer::getColorBuffer() const { return m_colorAttachment; }

void FrameBuffer::setSize(int width, int height) {
  bind();
  m_colorAttachment.setSize(width, height);

  glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  unbind();
}
} // namespace libs::renderer