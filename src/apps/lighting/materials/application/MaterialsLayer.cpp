#include "MaterialsLayer.hpp"

#include "../../mesh.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/Camera.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <format>

MaterialsLayer::MaterialsLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::Camera>(
                       glm::vec3(0.f, 0.0f, 5.0f))) {

  const auto shaderDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          MATERIALS_RESOURCES_FOLDER / "shaders")
                             .string();

  m_shaderManager.addShader("cube", std::format("{}/materials.vert", shaderDir),
                            std::format("{}/materials.frag", shaderDir));

  m_shaderManager.addShader(
      "light",
      (m_shaderManager.getCommonShaderDirectory() / "basicShader.vert")
          .string(),
      std::format("{}/light.frag", shaderDir));

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

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

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

void MaterialsLayer::onUpdate() {
  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

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

void MaterialsLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(MaterialsLayer::onWindowResized));

  dispatcher.dispatch<libs::events::KeyPressedEvent>(
      LOGL_BIND_EVENT_FN(MaterialsLayer::onKeyPressed));

  dispatcher.dispatch<libs::events::MouseMouvedEvent>(
      LOGL_BIND_EVENT_FN(MaterialsLayer::onMouseMoved));

  dispatcher.dispatch<libs::events::MouseScrolledEvent>(
      LOGL_BIND_EVENT_FN(MaterialsLayer::onMouseScrolled));
  // Handle events here
}

bool MaterialsLayer::onWindowResized(libs::events::WindowResizeEvent &event) {
  m_aspectRatio = static_cast<float>(event.getWidth()) /
                  static_cast<float>(event.getHeight());
  return event.handle();
}

bool MaterialsLayer::onKeyPressed(libs::events::KeyPressedEvent &event) {
  switch (event.getKeyCode()) {
  case GLFW_KEY_W:
    m_camera->processKeyboardInput(libs::renderer::CameraMovement::FORWARD,
                                   m_deltaTime);
    return event.handle();
  case GLFW_KEY_S:
    m_camera->processKeyboardInput(libs::renderer::CameraMovement::BACKWARD,
                                   m_deltaTime);
    return event.handle();
  case GLFW_KEY_A:
    m_camera->processKeyboardInput(libs::renderer::CameraMovement::LEFT,
                                   m_deltaTime);
    return event.handle();
  case GLFW_KEY_D:
    m_camera->processKeyboardInput(libs::renderer::CameraMovement::RIGHT,
                                   m_deltaTime);
    return event.handle();
  default:
    return false;
  }
}

bool MaterialsLayer::onMouseMoved(libs::events::MouseMouvedEvent &event) {
  static bool firstMouse{true};
  static float lastX{0.f}, lastY{0.f};

  if (firstMouse) {
    lastX = static_cast<float>(event.getXPos());
    lastY = static_cast<float>(event.getYPos());
    firstMouse = false;
  }

  float xOffset = static_cast<float>(event.getXPos()) - lastX;
  float yOffset = lastY - static_cast<float>(event.getYPos());

  lastX = static_cast<float>(event.getXPos());
  lastY = static_cast<float>(event.getYPos());

  m_camera->processMouseMovement(xOffset, yOffset);
  return event.handle();
}

bool MaterialsLayer::onMouseScrolled(libs::events::MouseScrolledEvent &event) {

  m_camera->processMouseScroll(event.getYOffset());
  return event.handle();
}

void MaterialsLayer::updateShaderCube() {
  glm::mat4 projection = glm::perspective(glm::radians(m_camera->getZoom()),
                                          m_aspectRatio, 0.1f, 100.f);

  auto view = m_camera->getViewMatrix();

  auto shader = m_shaderManager.getShader("cube");

  shader->use();
  shader->setMat4f("model", glm::mat4(1.0f));
  shader->setMat4f("view", view);
  shader->setMat4f("projection", projection);
  shader->setVec3f("material.ambient", 1.f, 0.5f, 0.31f);
  shader->setVec3f("material.diffuse", 1.f, 0.5f, 0.31f);
  shader->setVec3f("material.specular", 0.5f, 0.5f, 0.5f);
  shader->setFloat("material.shininess", 32.f);

  shader->setVec3f("light.color", m_lightColor);
  shader->setVec3f("light.position", m_lightPos);
  shader->setVec3f("light.ambient", glm::vec3(0.2f));
  shader->setVec3f("light.diffuse", glm::vec3(0.5f));
  shader->setVec3f("light.specular", glm::vec3(1.f));

  shader->setVec3f("viewPos", m_camera->getPosition());
}

void MaterialsLayer::updateShaderLight() {
  glm::mat4 projection = glm::perspective(glm::radians(m_camera->getZoom()),
                                          m_aspectRatio, 0.1f, 100.f);

  auto shader = m_shaderManager.getShader("light");

  shader->use();
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, m_lightPos);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  shader->setVec3f("lightColor", m_lightColor);
  shader->setMat4f("model", lightModel);
  shader->setMat4f("projection", projection);
  shader->setMat4f("view", m_camera->getViewMatrix());
}