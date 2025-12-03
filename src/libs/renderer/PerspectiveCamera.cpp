#include <libs/renderer/PerspectiveCamera.hpp>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace libs::renderer {
PerspectiveCamera::PerspectiveCamera(const glm::vec3 &position,
                                     float viewportWidth, float viewportHeight,
                                     const glm::vec3 &front, float yaw,
                                     float pitch)
    : Camera(position, viewportWidth, viewportHeight), m_front(front),
      m_up(m_worldUp), m_yaw(yaw), m_pitch(pitch) {
  update();
}

void PerspectiveCamera::setFov(float fov) {
  m_fov = std::max(std::min(fov, m_maxFov), m_minFov);
  ;
  updateProjection();
}

void PerspectiveCamera::adjustFov(float delta) { setFov(m_fov + delta); }

float PerspectiveCamera::getFov() const { return m_fov; }

void PerspectiveCamera::translate(const glm::vec3 &offset) {
  m_position += offset;
  updateView();
}

void PerspectiveCamera::translate(CameraMovement direction, float velocity) {
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
  default:
    break;
  }

  updateView();
}

void PerspectiveCamera::rotate(float xOffset, float yOffset) {
  m_yaw += xOffset;
  m_pitch += yOffset;

  if (m_constrainPitch) {
    m_pitch = std::max(std::min(m_pitch, 89.0f), -89.0f);
  }

  updateView();
}

void PerspectiveCamera::setViewportSize(float width, float height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
  updateProjection();
}

void PerspectiveCamera::update() {
  updateView();
  updateProjection();
}

void PerspectiveCamera::updateView() {
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

void PerspectiveCamera::updateProjection() {
  m_projection = glm::perspective(
      glm::radians(m_fov), m_viewportWidth / m_viewportHeight, m_near, m_far);
}
} // namespace libs::renderer