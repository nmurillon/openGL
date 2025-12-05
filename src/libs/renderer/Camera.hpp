#pragma once

#include <logl/renderer/export.h>

#include <glm/glm.hpp>

namespace libs::renderer {

enum class LOGL_RENDERER_EXPORT CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN,
};

class LOGL_RENDERER_EXPORT Camera {
public:
  Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
         const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f),
         float yaw = -90.f, float pitch = 0.f, float viewportWidth = 800.0f,
         float viewportHeight = 600.0f)
      : m_position(position), m_front(front), m_up(m_worldUp), m_yaw(yaw),
        m_pitch(pitch), m_viewportWidth(viewportWidth),
        m_viewportHeight(viewportHeight){};

  virtual ~Camera() = default;

  void translate(const glm::vec3 &offset);
  void translate(CameraMovement direction, float velocity);
  void rotate(float xOffset, float yOffset);
  void setViewportSize(float width, float height);
  virtual void setZoom(float zoom){};
  virtual void adjustZoom(float delta){};

  [[nodiscard]] const glm::vec3 &getPosition() const { return m_position; };
  [[nodiscard]] const glm::mat4 &getViewMatrix() const { return m_view; };
  [[nodiscard]] const glm::mat4 &getProjection() const { return m_projection; };

protected:
  virtual void update();
  virtual void updateView();
  virtual void updateProjection() = 0;

  glm::vec3 m_position;

  glm::mat4 m_view = glm::mat4(1.0f);
  glm::mat4 m_projection = glm::mat4(1.0f);

  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;

  glm::vec3 m_worldUp{0.0f, 1.0f, 0.0f};

  bool m_constrainPitch{false};
  float m_yaw;
  float m_pitch;

  float m_near{0.1f};
  float m_far{100.f};

  float m_viewportWidth;
  float m_viewportHeight;
};
} // namespace libs::renderer