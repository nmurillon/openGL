#include <libs/core/OpenGlStateCache.hpp>

namespace libs::core {
std::shared_ptr<OpenGlStateCache> OpenGlStateCache::getInstance() {
  static std::shared_ptr<OpenGlStateCache> instance{nullptr};

  if (!instance) {
    instance = std::make_shared<OpenGlStateCache>();
  }

  return instance;
}

void OpenGlStateCache::setViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

void OpenGlStateCache::clear(GLbitfield mask) { glClear(mask); }

void OpenGlStateCache::setClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void OpenGlStateCache::setClearColor(const RgbaColor &color) {
  glClearColor(color.red, color.green, color.blue, color.alpha);
}

void OpenGlStateCache::setDepthTest(bool enabled) {
  if (m_depthTestEnabled == enabled) {
    return;
  }

  m_depthTestEnabled = enabled;
  if (enabled) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void OpenGlStateCache::setBlend(bool enabled) {
  if (m_blendEnabled == enabled) {
    return;
  }

  m_blendEnabled = enabled;
  if (enabled) {
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }
}

void OpenGlStateCache::setStencilTest(bool enabled) {
  if (m_stencilTestEnabled == enabled) {
    return;
  }

  m_stencilTestEnabled = enabled;
  if (enabled) {
    glEnable(GL_STENCIL_TEST);
  } else {
    glDisable(GL_STENCIL_TEST);
  }
}

void OpenGlStateCache::setFaceCulling(bool enabled) {
  if (m_cullFaceEnabled == enabled) {
    return;
  }

  m_cullFaceEnabled = enabled;
  if (enabled) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

void OpenGlStateCache::setCullFace(GLenum face) {
  if (m_cullFaceEnabled && m_cullFace != face) {
    m_cullFace = face;
    glCullFace(face);
  }
}

void OpenGlStateCache::setFrontFaceWinding(GLenum winding) {
  if (m_cullFaceEnabled && m_frontFaceWinding != winding) {
    m_frontFaceWinding = winding;
    glFrontFace(winding);
  }
}

void OpenGlStateCache::setActiveTexture(GLenum texture) {
  if (m_activeTexture == texture) {
    return;
  }

  m_activeTexture = texture;
  glActiveTexture(texture);
}

void OpenGlStateCache::setActiveTexture(int texture) {
  if (texture < 0 || GL_TEXTURE0 + texture > GL_TEXTURE31) {
    return;
  }

  if (m_activeTexture == GL_TEXTURE0 + texture) {
    return;
  }

  m_activeTexture = GL_TEXTURE0 + texture;
  glActiveTexture(m_activeTexture);
}
} // namespace libs::core