#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace libs::core {
class Window {
public:
  Window(int width, int height, std::string title);
  ~Window();

  void open(std::function<void(void)> frameHandler);
  void setUserData(void *data);
  void setScrollCallback(GLFWscrollfun callback);
  void setCursorPosCallback(GLFWcursorposfun callback);
  void
  setKeyboardInputCallback(std::function<void(GLFWwindow *, double)> callback);

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