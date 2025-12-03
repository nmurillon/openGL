#include "ColorAppLayer.hpp"

#include "../../mesh.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <format>
#include <iostream>

std::string lightTypeToString(LightType type) {
  switch (type) {
  case LightType::Ambient:
    return "ambientLightSource";
  case LightType::Diffuse:
    return "diffuseLightSource";
  case LightType::Specular:
    return "specularLightSource";
  case LightType::SpecularViewSpace:
    return "specularLightSourceViewSpace";
  case LightType::Gouraud:
    return "gouraudLighting";
  default:
    return "Unknown";
  }
}

ColorAppLayer::ColorAppLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
                       glm::vec3(0.f, 0.0f, 5.0f))),
      m_cameraController(m_camera) {

  // TODO: fix this for ambient and diffuse
  const auto shaderDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          COLORS_RESOURCES_FOLDER / "shaders")
                             .string();

  m_shaderManager.addShader(
      "cube",
      std::format("{}/{}.vert", shaderDir, lightTypeToString(m_lightType)),
      std::format("{}/{}.frag", shaderDir, lightTypeToString(m_lightType)));

  m_shaderManager.addShader(
      "light",
      (m_shaderManager.getCommonShaderDirectory() / "basicShader.vert")
          .string(),
      std::format("{}/light.frag", shaderDir));

  std::cout << std::format("Using light type: {}",
                           lightTypeToString(m_lightType))
            << std::endl;

  glGenVertexArrays(1, &m_vaoCube);
  glGenVertexArrays(1, &m_vaoLight);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);

  // Cube data
  glBindVertexArray(m_vaoCube);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(
      GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(mesh::lighting::withNormals::vertices.size() *
                              sizeof(float)),
      mesh::lighting::withNormals::vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(mesh::lighting::withNormals::indices.size() *
                              sizeof(unsigned int)),
      mesh::lighting::withNormals::indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));

  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(3);

  // Light data
  glBindVertexArray(m_vaoLight);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glEnable(GL_DEPTH_TEST);
}

void ColorAppLayer::onUpdate() {
  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  m_cameraController.update();

  m_lightPos = glm::vec3(1.2f * cos(currentFrame), sin(currentFrame),
                         2.0f * cos(currentFrame));

  // Cube
  updateShaderCube();

  glBindVertexArray(m_vaoCube);
  glDrawElements(
      GL_TRIANGLES,
      static_cast<GLsizei>(mesh::lighting::withNormals::vertices.size()),
      GL_UNSIGNED_INT, 0);

  // Light
  updateShaderLight();

  glBindVertexArray(m_vaoLight);
  glDrawElements(
      GL_TRIANGLES,
      static_cast<GLsizei>(mesh::lighting::withNormals::vertices.size()),
      GL_UNSIGNED_INT, 0);
}

void ColorAppLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(ColorAppLayer::onWindowResized));

  m_cameraController.onEvent(event);
}

bool ColorAppLayer::onWindowResized(libs::events::WindowResizeEvent &event) {
  m_aspectRatio = static_cast<float>(event.getWidth()) /
                  static_cast<float>(event.getHeight());
  return event.handle();
}

void ColorAppLayer::updateShaderCube() {
  const glm::mat4 projection = m_camera->getProjection();

  auto shader = m_shaderManager.getShader("cube");
  shader->use();
  shader->setMat4f("model", glm::mat4(1.0f));
  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setMat4f("projection", projection);
  shader->setVec3f("lightColor", 1.f, 1.f, 1.f);
  shader->setVec3f("objectColor", 1.f, 0.5f, 0.31f);

  if (m_lightType == LightType::Diffuse) {
    shader->setVec3f("lightPos", m_lightPos);
  }

  if (m_lightType == LightType::Specular) {
    shader->setVec3f("lightPos", m_lightPos);
  }

  if (m_lightType == LightType::SpecularViewSpace) {
    shader->setVec3f("lightPosition", m_lightPos);
  }

  if (m_lightType == LightType::Gouraud) {
    shader->setVec3f("lightPos", m_lightPos);
  }

  shader->setVec3f("viewPos", m_camera->getPosition());
}

void ColorAppLayer::updateShaderLight() {
  const glm::mat4 projection = m_camera->getProjection();

  auto shader = m_shaderManager.getShader("light");

  shader->use();
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, m_lightPos);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  shader->setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
  shader->setMat4f("model", lightModel);
  shader->setMat4f("projection", projection);
  shader->setMat4f("view", m_camera->getViewMatrix());
}