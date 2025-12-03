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
         float viewportWidth = 800.0f, float viewportHeight = 600.0f)
      : m_position(position), m_viewportWidth(viewportWidth),
        m_viewportHeight(viewportHeight){};

  virtual ~Camera() = default;

  [[nodiscard]] const glm::vec3 &getPosition() const { return m_position; };
  [[nodiscard]] const glm::mat4 &getViewMatrix() const { return m_view; };
  [[nodiscard]] const glm::mat4 &getProjection() const { return m_projection; };

protected:
  virtual void update() = 0;

  glm::vec3 m_position;

  glm::mat4 m_view = glm::mat4(1.0f);
  glm::mat4 m_projection = glm::mat4(1.0f);

  float m_near{0.1f};
  float m_far{100.f};

  float m_viewportWidth;
  float m_viewportHeight;
};
} // namespace libs::renderer