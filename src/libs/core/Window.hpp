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

private:
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  GLFWwindow *m_window;
};
} // namespace libs::core