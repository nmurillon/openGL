#include <libs/renderer/PerspectiveCamera.hpp>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace libs::renderer {
PerspectiveCamera::PerspectiveCamera(const glm::vec3 &position,
                                     const glm::vec3 &front, float yaw,
                                     float pitch, float viewportWidth,
                                     float viewportHeight)
    : Camera(position, front, yaw, pitch, viewportWidth, viewportHeight) {
  update();
}

void PerspectiveCamera::setZoom(float zoom) { setFov(zoom); }

void PerspectiveCamera::adjustZoom(float delta) { setFov(m_fov + delta); }

void PerspectiveCamera::setFov(float fov) {
  m_fov = std::max(std::min(fov, m_maxFov), m_minFov);
  ;
  updateProjection();
}

void PerspectiveCamera::adjustFov(float delta) { setFov(m_fov + delta); }

float PerspectiveCamera::getFov() const { return m_fov; }

void PerspectiveCamera::updateProjection() {
  m_projection = glm::perspective(
      glm::radians(m_fov), m_viewportWidth / m_viewportHeight, m_near, m_far);
}
} // namespace libs::renderer