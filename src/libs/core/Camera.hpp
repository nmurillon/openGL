#pragma once

#include <logl/core/export.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace libs::core {

enum class LOGL_CORE_EXPORT CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

class LOGL_CORE_EXPORT Camera {
public:
  Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
         const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f),
         const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f,
         float pitch = 0.f);

  glm::mat4 getViewMatrix() const;
  void processMouseMovement(float xOffset, float yOffset,
                            GLboolean constraintPitch = true);
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
} // namespace libs::core