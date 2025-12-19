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

  virtual void onEvent(libs::events::Event &event) = 0;
  void onUpdate();
  //   virtual void onUpdate() = 0;
  virtual void onImguiUpdate(){};

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

private:
  bool m_isActive{true};
};
} // namespace libs::core