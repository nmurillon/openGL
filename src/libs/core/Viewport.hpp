#pragma once

#include <logl/core/export.h>

#include <libs/events/Event.hpp>

#include <string>

namespace libs::core {
class LOGL_CORE_EXPORT Viewport {
public:
  Viewport(const std::string &name, float width = 800.f, float height = 600.f);
  virtual ~Viewport() = default;
  void display();
  [[nodiscard]] bool isActive();

  /**
   * Check if a point is within the viewport. (0,0) is the bottom-left corner of
   * the window
   */
  [[nodiscard]] bool isInViewport(float x, float y);

  virtual void onEvent(libs::events::Event &event) = 0;
  void onUpdate();
  //   virtual void onUpdate() = 0;
  virtual void onImguiUpdate(){};

  static void setWindowSize(float width, float height);

protected:
  virtual void drawScene() = 0;

private:
  void prepareScene();

protected:
  std::string m_name;

  float m_width;
  float m_height;

  // Coords of the view port
  float m_xBottomLeft;
  float m_yBottomLeft;

  static float s_windowWidth;
  static float s_windowHeight;

private:
  bool m_isActive{true};
};
} // namespace libs::core