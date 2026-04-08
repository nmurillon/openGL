#pragma once

#include <logl/core/export.h>

#include <libs/core/Color.hpp>
#include <libs/core/OpenGlStateCache.hpp>
#include <libs/events/Event.hpp>

#include <memory>
#include <string>

namespace libs::core {
class LOGL_CORE_EXPORT Viewport {
public:
  Viewport(const std::string &name, float width = 800.f, float height = 600.f,
           const RgbaColor &backgroundColor = RgbaColor{0.1, 0.1, 0.1, 1.0});
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
  virtual void onImguiUpdate() {};

  static void setWindowSize(float width, float height);

  void setBackgroundColor(const RgbaColor &color);
  void setBackgroundColor(float r, float g, float b, float a);

protected:
  virtual void drawScene() = 0;
  virtual void initState() = 0;
  virtual void resetState() = 0;
  virtual void onViewportResize(float newWidth, float newHeight) {}

protected:
  std::string m_name;
  std::shared_ptr<libs::core::OpenGlStateCache> m_openglStateCache{
      libs::core::OpenGlStateCache::getInstance()};

  float m_width;
  float m_height;

  // Coords of the view port
  float m_xBottomLeft;
  float m_yBottomLeft;

  static float s_windowWidth;
  static float s_windowHeight;

  // Background color
  RgbaColor m_backgroundColor{0.1, 0.1, 0.1, 1.0};

private:
  bool m_isActive{true};
};
} // namespace libs::core