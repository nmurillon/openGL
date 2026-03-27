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

  void setBlend(bool enabled);

  void setStencilTest(bool enabled);

  void setFaceCulling(bool enabled);
  void setCullFace(GLenum face);
  void setFrontFaceWinding(GLenum winding);

  void setActiveTexture(GLenum texture);
  void setActiveTexture(int texture);

private:
  // Everyting related to opengl state
  bool m_depthTestEnabled{false};
  bool m_blendEnabled{false};

  // Related to stencil
  bool m_stencilTestEnabled{false};

  // Related to face culling
  bool m_cullFaceEnabled{false};
  GLenum m_cullFace{GL_BACK};
  GLenum m_frontFaceWinding{GL_CCW};

  GLenum m_activeTexture{GL_TEXTURE0};
};
} // namespace libs::core