#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Camera.hpp>

// TODO: add methods to rotate and translate
namespace libs::renderer {
class LOGL_RENDERER_EXPORT PerspectiveCamera : public Camera {
public:
  PerspectiveCamera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
                    float viewportWidth = 800.0f, float viewportHeight = 600.0f,
                    const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f),
                    float yaw = -90.f, float pitch = 0.f);

  void setFov(float fov);
  float getFov() const;
  void translate(const glm::vec3 &offset);
  void rotate(float xOffset, float yOffset);
  void setViewportSize(float width, float height);

protected:
  void update() override;

private:
  void updateView();
  void updateProjection();

private:
  bool m_constrainPitch{false};
  float m_yaw;
  float m_pitch;

  float m_fov{45.0f};
  float m_minFov{0.0f};
  float m_maxFov{110.0f};

  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;

  glm::vec3 m_worldUp{0.0f, 1.0f, 0.0f};
};
} // namespace libs::renderer