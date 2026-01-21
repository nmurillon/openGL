#include <libs/renderer/MouseCameraController.hpp>

#include <libs/events/EventDispatcher.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace libs::renderer {
MouseCameraController::MouseCameraController(std::shared_ptr<Camera> camera)
    : m_camera(std::move(camera)), m_lastFrame(glfwGetTime()) {}

void MouseCameraController::update() {
  if (!m_isEnabled) {
    return;
  }

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

void MouseCameraController::onEvent(libs::events::Event &event) {
  if (!m_isEnabled && !m_isDragging) {
    return;
  }

  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::MouseMovedEvent>(
      LOGL_BIND_EVENT_FN(MouseCameraController::onMouseMoved));

  dispatcher.dispatch<libs::events::MouseScrolledEvent>(
      LOGL_BIND_EVENT_FN(MouseCameraController::onMouseScrolled));

  dispatcher.dispatch<libs::events::MouseButtonPressedEvent>(
      LOGL_BIND_EVENT_FN(MouseCameraController::onMouseButtonPressed));

  dispatcher.dispatch<libs::events::MouseButtonReleasedEvent>(
      LOGL_BIND_EVENT_FN(MouseCameraController::onMouseButtonReleased));
};

void MouseCameraController::setCamera(std::shared_ptr<Camera> camera) {
  m_camera = std::move(camera);
};

void MouseCameraController::setEnabled(bool enabled) { m_isEnabled = enabled; }

// TODO: logic when mouse is dragged
void MouseCameraController::onMouseButtonPressed(
    libs::events::MouseButtonPressedEvent &event) {
  m_isDragging = true;
  event.handle();
}

void MouseCameraController::onMouseButtonReleased(
    libs::events::MouseButtonReleasedEvent &event) {
  m_isDragging = false;
  m_firstMouse = true;
  event.handle();
}

void MouseCameraController::onMouseMoved(libs::events::MouseMovedEvent &event) {
  if (!m_isDragging) {
    return;
  }

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

void MouseCameraController::onMouseScrolled(
    libs::events::MouseScrolledEvent &event) {
  m_camera->adjustZoom(-static_cast<float>(event.getYOffset()));

  event.handle();
}

} // namespace libs::renderer