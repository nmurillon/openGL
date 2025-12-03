#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Camera.hpp>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT PerspectiveCamera : public Camera {
public:
  PerspectiveCamera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
                    const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f),
                    float yaw = -90.f, float pitch = 0.f,
                    float viewportWidth = 800.0f,
                    float viewportHeight = 600.0f);

  void setZoom(float zoom) override;
  void adjustZoom(float delta) override;
  void setFov(float fov);
  void adjustFov(float delta);
  float getFov() const;

  void translate(const glm::vec3 &offset);
  void translate(CameraMovement direction, float velocity);
  void rotate(float xOffset, float yOffset);
  void setViewportSize(float width, float height);

protected:
  void updateProjection() override;

private:
  float m_fov{45.0f};
  float m_minFov{0.0f};
  float m_maxFov{110.0f};
};
} // namespace libs::renderer