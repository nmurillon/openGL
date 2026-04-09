#include "UniformBufferViewport.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <libs/renderer/PerspectiveCamera.hpp>

#include <format>

UniformBufferViewport::UniformBufferViewport(
    const std::string &name, float width, float height,
    const std::filesystem::path &assetsDir)
    : Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  for (const auto &color : s_colors) {
    m_shaderManager.addShader(color, m_assetsDir / "cube.vert",
                              m_assetsDir / std::format("{}.frag", color));

    const auto shader = m_shaderManager.getShader(color);
    shader->setBindingPoint("Matrices", 0);
  }

  m_uniformBuffer.setBindingPoint(0);
}

void UniformBufferViewport::initState() {
  m_openglStateCache->setDepthTest(true);
}

void UniformBufferViewport::resetState() {
  m_openglStateCache->setDepthTest(false);
}

void UniformBufferViewport::onEvent(libs::events::Event &event) {
  if (isActive()) {
    m_cameraController.onEvent(event);
  }
}

void UniformBufferViewport::onViewportResize(float newWidth, float newHeight) {
  m_camera->setViewportSize(newWidth, newHeight);
}

void UniformBufferViewport::drawScene() {
  m_cameraController.update();
  m_uniformBuffer.setData(0, sizeof(glm::mat4),
                          glm::value_ptr(m_camera->getProjection()));
  m_uniformBuffer.setData(sizeof(glm::mat4), sizeof(glm::mat4),
                          glm::value_ptr(m_camera->getViewMatrix()));

  m_openglStateCache->setClearColor(0.1f, 0.1f, 0.1f, 0.1f);

  int i = 0;
  for (const auto &color : s_colors) {
    const auto shader = m_shaderManager.getShader(color);
    shader->use();

    glm::mat4 model(1.0);
    model = glm::translate(model, s_positions.at(i++));
    shader->setMat4f("model", model);

    m_cube.draw();
  }
}