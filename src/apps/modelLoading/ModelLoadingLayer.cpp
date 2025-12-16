#include "ModelLoadingLayer.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>
#include <imgui/imgui.h>

#include <cmath>

ModelLoadingLayer::ModelLoadingLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
                       glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto shaderDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          MODELLOADING_RESOURCES_FOLDER / "assets" / "shaders")
                             .string();

  m_shaderManager.addShader(
      "lightSource",
      (m_shaderManager.getCommonShaderDirectory() / "basicShader.vert")
          .string(),
      std::format("{}/light.frag", shaderDir));

  m_shaderManager.addShader("model", std::format("{}/model.vert", shaderDir),
                            std::format("{}/model.frag", shaderDir));

  glGenVertexArrays(1, &m_vaoLight);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);

  // Light data
  glBindVertexArray(m_vaoLight);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        reinterpret_cast<void *>(0));

  glEnableVertexAttribArray(0);

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glEnable(GL_DEPTH_TEST);
}

void ModelLoadingLayer::onUpdate() {
  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  m_cameraController.update();

  auto shader = m_shaderManager.getShader("model");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.f));
  model = glm::scale(model, glm::vec3(1.0f));
  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());
  for (const auto &[path, model] : m_models) {
    model.draw(*shader);
  }

  // Light
  updateShaderLight();
}

void ModelLoadingLayer::onImguiUpdate() {}

void ModelLoadingLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(ModelLoadingLayer::onWindowResized));

  dispatcher.dispatch<libs::events::PathDropEvent>(
      LOGL_BIND_EVENT_FN(ModelLoadingLayer::onPathDropped));

  m_cameraController.onEvent(event);
}

bool ModelLoadingLayer::onWindowResized(
    libs::events::WindowResizeEvent &event) {
  float width = static_cast<float>(event.getWidth());
  float height = static_cast<float>(event.getHeight());

  m_aspectRatio = width / height;

  m_camera->setViewportSize(width, height);
  return event.handle();
}

void ModelLoadingLayer::onPathDropped(libs::events::PathDropEvent &event) {
  for (const auto &path : event.getPaths()) {
    if (m_models.find(path) == m_models.end()) {
      const auto model = libs::renderer::Model{path};
      if (model.isValid()) {
        m_models.insert({path, model}); // TODO: move semantics
      }
    }
  }
}

void ModelLoadingLayer::updateShaderLight() {
  auto shader = m_shaderManager.getShader("lightSource");

  shader->use();
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, m_light.position);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  shader->setVec3f("lightColor", m_light.color);
  shader->setMat4f("model", lightModel);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());

  glBindVertexArray(m_vaoLight);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                 GL_UNSIGNED_INT, 0);
}