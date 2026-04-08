#pragma once

#include <logl/renderer/export.h>

#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/renderer/Camera.hpp>

#include <memory>

// TODO: Rework this class to have something looking like blender
namespace libs::renderer {
class LOGL_RENDERER_EXPORT MouseCameraController {
public:
  MouseCameraController() = default;
  MouseCameraController(std::shared_ptr<Camera> camera);
  MouseCameraController(const MouseCameraController &other) = delete;
  MouseCameraController(MouseCameraController &&other) = delete;
  MouseCameraController &operator=(const MouseCameraController &other) = delete;
  MouseCameraController &operator=(MouseCameraController &&other) = delete;
  ~MouseCameraController() = default;

  void update();
  void onEvent(libs::events::Event &event);
  void setCamera(std::shared_ptr<Camera> camera);
  void setEnabled(bool enabled);

private:
  void onMouseMoved(libs::events::MouseMovedEvent &event);
  void onMouseScrolled(libs::events::MouseScrolledEvent &event);
  void onMouseButtonPressed(libs::events::MouseButtonPressedEvent &event);
  void onMouseButtonReleased(libs::events::MouseButtonReleasedEvent &event);

private:
  std::shared_ptr<Camera> m_camera;
  bool m_isEnabled{true};

  float m_mouseSensitivity{0.1f};
  float m_movementSpeed{2.5f};
  bool m_firstMouse{true};
  float m_lastX{0.0f};
  float m_lastY{0.0f};
  bool m_isDragging{false};

  double m_deltaTime{0.0f};
  double m_lastFrame{0.0f};
};
} // namespace libs::renderer