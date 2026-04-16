#include "ExploadingModelViewport.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <format>

ExploadingModelViewport::ExploadingModelViewport(
    const std::string &name, float width, float height,
    const std::filesystem::path &assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 10.0f))) {

  const auto temp = (m_assetsDir / "expload").string();
  m_shaderManager.addShader("expload", std::format("{}.vert", temp),
                            std::format("{}.frag", temp),
                            std::format("{}.geom", temp));
}

void ExploadingModelViewport::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);

  dispatcher.dispatch<libs::events::PathDropEvent>(
      LOGL_BIND_EVENT_FN(ExploadingModelViewport::onPathDropped));
}

void ExploadingModelViewport::onPathDropped(
    libs::events::PathDropEvent &event) {
  const auto path = event.getPaths().front();
  auto model = libs::renderer::Model(path);

  if (model.isValid()) {
    m_model = std::move(model);
  }
}

void ExploadingModelViewport::initState() {
  m_openglStateCache->setDepthTest(true);
}

void ExploadingModelViewport::resetState() {
  m_openglStateCache->setDepthTest(false);
}

void ExploadingModelViewport::drawScene() {
  const auto shader = m_shaderManager.getShader("expload");
  shader->use();

  const auto time = glfwGetTime();
  shader->setFloat("iTime", time);

  glm::mat4 model{1.0};

  shader->setMat4f("model", model);
  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setMat4f("projection", m_camera->getProjection());

  if (m_model.has_value() && m_model->isValid()) {
    rotateCamera(time);
    m_model->draw(*shader);
  }
}

void ExploadingModelViewport::rotateCamera(float time) {
  float angle = 2.0 * 3.14 * time / 5.f;
  float height = 5.0f * sin(time * 0.3f);
  glm::vec3 position{10.0f * cos(angle), height, 10.0f * sin(angle)};

  m_camera->setPosition(position);
  m_camera->lookAt(glm::vec3{0.f});
}