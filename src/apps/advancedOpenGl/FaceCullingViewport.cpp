#include "FaceCullingViewport.hpp"

#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>

FaceCullingViewport::FaceCullingViewport(const std::string &name, float width,
                                         float height,
                                         const std::filesystem::path &assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto shaderDir = (m_assetsDir / "shaders").string();
  const auto textureDir = m_assetsDir / "textures";

  m_metal = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/metal.png", textureDir.string())};

  m_shaderManager.addShader("cube", std::format("{}/cube.vert", shaderDir),
                            std::format("{}/cube.frag", shaderDir));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_metal.id());

  glEnable(GL_CULL_FACE);
}

void FaceCullingViewport::onEvent(libs::events::Event &event) {
  m_cameraController.onEvent(event);
}

void FaceCullingViewport::drawScene() {
  m_cameraController.update();

  auto shader = m_shaderManager.getShader("cube");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());

  shader->setInt("tex", 0);
  m_cube.draw();
}