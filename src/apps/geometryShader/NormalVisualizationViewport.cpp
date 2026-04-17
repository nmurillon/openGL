#include "NormalVisualizationViewport.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/type_ptr.hpp>

NormalVisualizationViewport::NormalVisualizationViewport(
    const std::string &name, float width, float height,
    const std::filesystem::path &assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.f, 10.f))) {
  m_shaderManager.addShader("model", m_assetsDir / "model.vert",
                            m_assetsDir / "model.frag");

  m_shaderManager.addShader("normal", m_assetsDir / "normalVis.vert",
                            m_assetsDir / "normalVis.frag",
                            m_assetsDir / "normalVis.geom");

  m_uniformBuffer.setBindingPoint(0);
  m_shaderManager.getShader("model")->setBindingPoint("Matrices", 0);
  m_shaderManager.getShader("normal")->setBindingPoint("Matrices", 0);
}

void NormalVisualizationViewport::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);

  dispatcher.dispatch<libs::events::PathDropEvent>(
      LOGL_BIND_EVENT_FN(NormalVisualizationViewport::onPathDropped));
}

void NormalVisualizationViewport::onImguiUpdate() {}

void NormalVisualizationViewport::onPathDropped(
    libs::events::PathDropEvent &event) {
  const auto path = event.getPaths().front();
  auto model = libs::renderer::Model(path);

  if (model.isValid()) {
    m_model = std::move(model);
  }
}

void NormalVisualizationViewport::initState() {
  m_openglStateCache->setDepthTest(true);
}

void NormalVisualizationViewport::resetState() {
  m_openglStateCache->setDepthTest(false);
}

void NormalVisualizationViewport::drawScene() {
  if (!m_model.has_value() || !m_model->isValid()) {
    return;
  }

  glm::mat4 model{1.0};

  const float time = glfwGetTime();
  const float angle = 2.0 * 3.14 * time / 5.f;
  const float height = 5.0f * sin(time * 0.3f);
  glm::vec3 position{10.0f * cos(angle), height, 10.0f * sin(angle)};

  m_camera->setPosition(position);
  m_camera->lookAt(glm::vec3{0.f});

  m_uniformBuffer.setData(0, sizeof(glm::mat4), glm::value_ptr(model));
  m_uniformBuffer.setData(sizeof(glm::mat4), sizeof(glm::mat4),
                          glm::value_ptr(m_camera->getViewMatrix()));
  m_uniformBuffer.setData(2 * sizeof(glm::mat4), sizeof(glm::mat4),
                          glm::value_ptr(m_camera->getProjection()));

  const auto shader = m_shaderManager.getShader("model");
  shader->use();

  m_model->draw(*shader);
  const auto a = glGetError();

  // Draw the normals
  const auto normShader = m_shaderManager.getShader("normal");
  normShader->use();
  m_model->draw(*normShader);
  const auto b = glGetError();
}