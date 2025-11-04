#pragma once

#include <logl/renderer/export.h>

#include <glm/glm.hpp>

namespace libs::renderer {

enum class LOGL_RENDERER_EXPORT CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

class LOGL_RENDERER_EXPORT Camera {
public:
  Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
         const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f),
         const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f,
         float pitch = 0.f);

  glm::mat4 getViewMatrix() const;
  const glm::vec3 &getPosition() const;
  void processMouseMovement(float xOffset, float yOffset,
                            bool constraintPitch = true);
  void processKeyboardInput(const CameraMovement &direction, float deltaTime);
  void processMouseScroll(float yOffset);

  const float &getZoom() const;

private:
  void update();

  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;

  glm::vec3 m_worldUp;

  float m_yaw;
  float m_pitch;

  float m_movementSpeed{2.5f};
  float m_mouseSensitivity{0.1f};
  float m_zoom{45.f};
};
} // namespace libs::renderer