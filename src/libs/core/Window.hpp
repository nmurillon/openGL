#pragma once

#include <logl/core/export.h>

#include <libs/events/WindowEvent.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace libs::core {
class LOGL_CORE_EXPORT Window {
public:
  using EventCallbackFn = std::function<void(events::Event &)>;

  Window(float width, float height, const std::string &title);
  ~Window();

  void setEventCallback(const EventCallbackFn &eventCallback);
  void onUpdate();
  void onEvent(events::Event &event);
  bool shouldClose() const;
  const int &getWidth() const;
  const int &getHeight() const;
  const double &getDeltaTime() const;
  double getFps() const;

private:
  EventCallbackFn m_eventCallback;
  bool onWindowResized(libs::events::WindowResizeEvent &event);

  GLFWwindow *m_window;
  float m_width;
  float m_height;
  // Time between two last frames
  double m_deltaTime;
};
} // namespace libs::core