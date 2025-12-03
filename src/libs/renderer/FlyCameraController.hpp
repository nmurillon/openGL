#pragma once

#include <logl/renderer/export.h>

#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <memory>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT FlyCameraController {
public:
  FlyCameraController() = default;
  FlyCameraController(std::shared_ptr<PerspectiveCamera> camera);
  FlyCameraController(const FlyCameraController &other) = delete;
  FlyCameraController(FlyCameraController &&other) = delete;
  FlyCameraController &operator=(const FlyCameraController &other) = delete;
  FlyCameraController &operator=(FlyCameraController &&other) = delete;
  ~FlyCameraController() = default;

  void update();
  void onEvent(libs::events::Event &event);
  void setCamera(std::shared_ptr<PerspectiveCamera> camera);

private:
  void onMouseMoved(libs::events::MouseMovedEvent &event);
  void onMouseScrolled(libs::events::MouseScrolledEvent &event);

private:
  std::shared_ptr<PerspectiveCamera> m_camera;

  float m_mouseSensitivity{0.1f};
  float m_movementSpeed{2.5f};
  bool m_firstMouse{true};
  float m_lastX{0.0f};
  float m_lastY{0.0f};

  double m_deltaTime{0.0f};
  double m_lastFrame{0.0f};
};
} // namespace libs::renderer