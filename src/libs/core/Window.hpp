#pragma once

#include <logl/core/export.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace libs::core {
class LOGL_CORE_EXPORT Window {
public:
  Window(int width, int height, const std::string &title);
  ~Window();

  void onUpdate();
  bool shouldClose() const;
  const int &getWidth() const;
  const int &getHeight() const;
  const double &getDeltaTime() const;
  const double getFps() const;

private:
  std::function<void(GLFWwindow *, double)> m_keyboardInputCallback;

  GLFWwindow *m_window;
  int m_width;
  int m_height;
  // Time between two last frames
  double m_deltaTime;
};
} // namespace libs::core