#include "ColorAppLayer.hpp"

#include "../../mesh.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Shader.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

ColorAppLayer::ColorAppLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::Camera>(
                       glm::vec3(0.f, 0.0f, 5.0f))),
      m_shaderCube("shaders/basicShader.vert",
                   "shaders/specularLightSource.frag"),
      m_shaderLight("shaders/basicShader.vert", "shaders/light.frag") {

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

  // Setup light shader data
  glm::mat4 lightModel = glm::mat4(1.0f);
  glm::vec3 lightPos = glm::vec3(0.f, 0.0f, 2.0f);
  lightModel = glm::translate(lightModel, lightPos);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  m_shaderLight.use();
  m_shaderLight.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
  m_shaderLight.setMat4f("model", lightModel);

  // Setup cube shader data
  glm::mat4 cubeModel = glm::mat4(1.0f);
  cubeModel = glm::translate(cubeModel, glm::vec3(1.2f, 1.0f, 2.0f));
  cubeModel = glm::scale(cubeModel, glm::vec3(0.2f));
  m_shaderCube.setVec3f("objectColor", 1.f, 0.5f, 0.31f);
  m_shaderCube.setVec3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
  m_shaderCube.setMat4f("model", glm::mat4(1.0f));

  glEnable(GL_DEPTH_TEST);
}

void ColorAppLayer::onUpdate() {
  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  // TODO: update aspect ratio with window resize
  glm::mat4 projection = glm::perspective(glm::radians(m_camera->getZoom()),
                                          m_aspectRatio, 0.1f, 100.f);

  glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

  m_shaderLight.setMat4f("view", m_camera->getViewMatrix());
  m_shaderLight.setMat4f("projection", projection);

  // Cube
  m_shaderCube.use();
  m_shaderCube.setVec3f("lightColor", 1.f, 1.f, 1.f);
  m_shaderCube.setVec3f("objectColor", 1.f, 0.5f, 0.31f);
  m_shaderCube.setMat4f("model", glm::mat4(1.0f));
  m_shaderCube.setMat4f("projection", projection);
  m_shaderCube.setVec3f("lightPos", lightPos.x, lightPos.y, lightPos.z);

  const auto cameraPos = m_camera->getPosition();
  m_shaderCube.setVec3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

  m_shaderCube.setMat4f("view", m_camera->getViewMatrix());

  glBindVertexArray(m_vaoCube);
  glDrawElements(
      GL_TRIANGLES,
      static_cast<GLsizei>(mesh::lighting::withNormals::vertices.size()),
      GL_UNSIGNED_INT, 0);

  // Light
  m_shaderLight.use();
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  m_shaderLight.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
  m_shaderLight.setMat4f("model", lightModel);
  m_shaderLight.setMat4f("projection", projection);
  m_shaderLight.setMat4f("view", m_camera->getViewMatrix());

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

  dispatcher.dispatch<libs::events::KeyPressedEvent>(
      LOGL_BIND_EVENT_FN(ColorAppLayer::onKeyPressed));

  dispatcher.dispatch<libs::events::MouseMouvedEvent>(
      LOGL_BIND_EVENT_FN(ColorAppLayer::onMouseMoved));

  dispatcher.dispatch<libs::events::MouseScrolledEvent>(
      LOGL_BIND_EVENT_FN(ColorAppLayer::onMouseScrolled));
  // Handle events here
}

bool ColorAppLayer::onWindowResized(libs::events::WindowResizeEvent &event) {
  m_aspectRatio = static_cast<float>(event.getWidth()) /
                  static_cast<float>(event.getHeight());
  return event.handle();
}

bool ColorAppLayer::onKeyPressed(libs::events::KeyPressedEvent &event) {
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

bool ColorAppLayer::onMouseMoved(libs::events::MouseMouvedEvent &event) {
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

bool ColorAppLayer::onMouseScrolled(libs::events::MouseScrolledEvent &event) {

  m_camera->processMouseScroll(event.getYOffset());
  return event.handle();
}