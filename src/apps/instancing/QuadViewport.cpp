#include "QuadViewport.hpp"

#include <libs/renderer/VertexBuffer.hpp>

QuadViewport::QuadViewport(const std::string &name, float width, float height,
                           std::filesystem::path assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir) {
  const auto textureDir = m_assetsDir / "textures";
  const auto shaderDir = m_assetsDir / "shaders";

  m_shaderManager.addShader("quad", shaderDir / "quad.vert",
                            shaderDir / "quad.frag");

  // Prepare vbo for offsets
  std::vector<glm::vec2> offsets;

  offsets.reserve(100);
  float offset = 0.1f;
  for (int y = -10; y < 10; y += 2) {
    for (int x = -10; x < 10; x += 2) {
      glm::vec2 translation;
      translation.x = static_cast<float>(x) / 10.0f + offset;
      translation.y = static_cast<float>(y) / 10.0f + offset;
      offsets.emplace_back(translation);
    }
  }

  m_quad.addVertexBuffer(
      {libs::renderer::BufferLayout{{sizeof(float), 2, GL_FLOAT, GL_FALSE, 1}},
       offsets.data(), offsets.size() * sizeof(glm::vec2)});
}

void QuadViewport::drawScene() {
  const auto shader = m_shaderManager.getShader("quad");
  shader->use();

  m_quad.draw(true, 100);
}