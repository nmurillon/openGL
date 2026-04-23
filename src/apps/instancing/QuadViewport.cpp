#include "QuadViewport.hpp"

QuadViewport::QuadViewport(const std::string &name, float width, float height,
                           std::filesystem::path assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir) {
  const auto textureDir = m_assetsDir / "textures";
  const auto shaderDir = m_assetsDir / "shaders";

  m_shaderManager.addShader("quad", shaderDir / "quad.vert",
                            shaderDir / "quad.frag");
}

void QuadViewport::drawScene() {
  const auto shader = m_shaderManager.getShader("quad");
  shader->use();

  m_quad.draw();
}