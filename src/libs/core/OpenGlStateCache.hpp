#pragma once

#include <logl/core/export.h>

#include <libs/core/Color.hpp>
#include <libs/openGl/opengl.h>

#include <memory>

namespace libs::core {
class LOGL_CORE_EXPORT OpenGlStateCache {
public:
  OpenGlStateCache() = default;
  ~OpenGlStateCache() = default;
  OpenGlStateCache(const OpenGlStateCache &) = delete;
  OpenGlStateCache &operator=(const OpenGlStateCache &) = delete;
  OpenGlStateCache(OpenGlStateCache &&) = delete;
  OpenGlStateCache &operator=(OpenGlStateCache &&) = delete;

  static std::shared_ptr<OpenGlStateCache> getInstance();

public:
  // Everyting related to opengl state
  void setViewport(int x, int y, int width, int height);
  void clear(GLbitfield mask);
  void setClearColor(float r, float g, float b, float a);
  void setClearColor(const RgbaColor &color);

  void setDepthTest(bool enabled);
  void setDepthFunc(GLenum func);
  void setDepthMask(bool allowWrite);

  void setBlend(bool enabled);
  void setBlendFunc(GLenum sfactor, GLenum dfactor);

  void setStencilTest(bool enabled);
  void setStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
  void setStencilMask(GLuint mask);
  void setStencilFunc(GLenum func, GLint ref, GLuint mask);

  void setFaceCulling(bool enabled);
  void setCullFace(GLenum face);
  void setFrontFaceWinding(GLenum winding);

  void setActiveTexture(GLenum texture);
  void setActiveTexture(int texture);
  int getBoundTexture(GLenum target) const;

private:
  // Everyting related to depth test
  bool m_depthTestEnabled{false};
  GLenum m_depthFunc{GL_LESS};
  bool m_depthAllowWrite{true};

  // Related to blending
  bool m_blendEnabled{false};
  GLenum m_blendSFactor{GL_ONE};
  GLenum m_blendDFactor{GL_ZERO};

  // Related to stencil
  bool m_stencilTestEnabled{false};
  GLenum m_stencilSFail{GL_KEEP};
  GLenum m_stencilDpFail{GL_KEEP};
  GLenum m_stencilDpPass{GL_KEEP};
  GLuint m_stencilMask{0xFF};

  // TODO: store stencil func?

  // Related to face culling
  bool m_cullFaceEnabled{false};
  GLenum m_cullFace{GL_BACK};
  GLenum m_frontFaceWinding{GL_CCW};

  GLenum m_activeTexture{GL_TEXTURE0};
};
} // namespace libs::core