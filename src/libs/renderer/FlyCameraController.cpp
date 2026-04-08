#include <libs/renderer/FlyCameraController.hpp>

#include <libs/events/EventDispatcher.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <libs/openGl/opengl.h>

namespace libs::renderer {
FlyCameraController::FlyCameraController(std::shared_ptr<Camera> camera)
    : m_camera(std::move(camera)), m_lastFrame(glfwGetTime()) {}

void FlyCameraController::update() {
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - m_lastFrame;
  m_lastFrame = currentFrame;

  const auto velocity = static_cast<float>(m_movementSpeed * m_deltaTime);

  // TODO: make generic function/class for inputs
  auto *window = glfwGetCurrentContext();
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    m_camera->translate(CameraMovement::FORWARD, velocity);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    m_camera->translate(CameraMovement::BACKWARD, velocity);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    m_camera->translate(CameraMovement::LEFT, velocity);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    m_camera->translate(CameraMovement::RIGHT, velocity);
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    m_camera->translate(CameraMovement::UP, velocity);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    m_camera->translate(CameraMovement::DOWN, velocity);
  }
}

void FlyCameraController::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::MouseMovedEvent>(
      LOGL_BIND_EVENT_FN(FlyCameraController::onMouseMoved));

  dispatcher.dispatch<libs::events::MouseScrolledEvent>(
      LOGL_BIND_EVENT_FN(FlyCameraController::onMouseScrolled));
};

void FlyCameraController::setCamera(std::shared_ptr<Camera> camera) {
  m_camera = std::move(camera);
};

void FlyCameraController::onMouseMoved(libs::events::MouseMovedEvent &event) {
  if (m_firstMouse) {
    m_firstMouse = false;
    m_lastX = static_cast<float>(event.getXPos());
    m_lastY = static_cast<float>(event.getYPos());
  }

  float xOffset = (event.getXPos() - m_lastX) * m_mouseSensitivity;
  float yOffset = (m_lastY - event.getYPos()) * m_mouseSensitivity;

  m_lastX = static_cast<float>(event.getXPos());
  m_lastY = static_cast<float>(event.getYPos());

  m_camera->rotate(xOffset, yOffset);

  event.handle();
}

void FlyCameraController::onMouseScrolled(
    libs::events::MouseScrolledEvent &event) {
  m_camera->adjustZoom(-static_cast<float>(event.getYOffset()));

  event.handle();
}

} // namespace libs::renderer