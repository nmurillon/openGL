#include "libs/core/Camera.hpp"

#include <algorithm>

namespace libs::core {
Camera::Camera(const glm::vec3 &position, const glm::vec3 &front,
               const glm::vec3 &up, float yaw, float pitch)
    : m_position(position), m_front(front), m_up(up),
      m_right(glm::normalize(glm::cross(front, up))), m_worldUp(up), m_yaw(yaw),
      m_pitch(pitch) {
  update();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processMouseMovement(float xOffset, float yOffset,
                                  GLboolean constraintPitch) {
  xOffset *= m_mouseSensitivity;
  yOffset *= m_mouseSensitivity;

  m_yaw += xOffset;
  m_pitch += yOffset;

  if (constraintPitch) {
    m_pitch = std::max(-89.f, std::min(89.f, m_pitch));
  }

  update();
}

void Camera::processKeyboardInput(const CameraMovement &direction,
                                  float deltaTime) {
  const float velocity = m_movementSpeed * deltaTime;
  glm::vec3 movement{0.f, 0.f, 0.f};
  if (direction == CameraMovement::FORWARD) {
    movement = m_front * velocity;
  }
  if (direction == CameraMovement::BACKWARD) {
    movement = -m_front * velocity;
  }
  if (direction == CameraMovement::LEFT) {
    movement = -m_right * velocity;
  }
  if (direction == CameraMovement::RIGHT) {
    movement = m_right * velocity;
  }

  m_position += movement;
}

void Camera::processMouseScroll(float yOffset) {
  m_zoom -= yOffset;
  if (m_zoom < 1.0f) {
    m_zoom = 1.0f;
  }
  if (m_zoom > 45.0f) {
    m_zoom = 45.0f;
  }
}

const float &Camera::getZoom() const { return m_zoom; }

void Camera::update() {
  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}
} // namespace libs::core