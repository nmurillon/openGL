#include <libs/renderer/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace libs::renderer {
void Camera::translate(const glm::vec3 &offset) {
  m_position += offset;
  updateView();
}

void Camera::translate(CameraMovement direction, float velocity) {
  switch (direction) {
  case CameraMovement::FORWARD:
    m_position += m_front * velocity;
    break;
  case CameraMovement::BACKWARD:
    m_position -= m_front * velocity;
    break;
  case CameraMovement::LEFT:
    m_position -= m_right * velocity;
    break;
  case CameraMovement::RIGHT:
    m_position += m_right * velocity;
    break;
  case CameraMovement::UP:
    m_position += m_up * velocity;
    break;
  case CameraMovement::DOWN:
    m_position -= m_up * velocity;
    break;
  default:
    break;
  }

  updateView();
}

void Camera::rotate(float xOffset, float yOffset) {
  m_yaw += xOffset;
  m_pitch += yOffset;

  if (m_constrainPitch) {
    m_pitch = std::max(std::min(m_pitch, 89.0f), -89.0f);
  }

  updateView();
}

void Camera::setViewportSize(float width, float height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
  updateProjection();
}

void Camera::update() {
  updateView();
  updateProjection();
}

void Camera::updateView() {
  // Update front vector based on yaw and pitch
  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);

  // Recalculate right and up vectors
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));

  // Update view matrix
  m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}
} // namespace libs::renderer