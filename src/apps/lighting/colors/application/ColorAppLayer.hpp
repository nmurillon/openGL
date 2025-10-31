#pragma once

#include <libs/core/Layer.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Shader.hpp>

#include <memory>

class ColorAppLayer : public libs::core::Layer {
public:
  ColorAppLayer(const std::string &name = "ColorAppLayer");

  virtual ~ColorAppLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;

private:
  bool onWindowResized(libs::events::WindowResizeEvent &event);
  bool onKeyPressed(libs::events::KeyPressedEvent &event);
  bool onMouseMoved(libs::events::MouseMouvedEvent &event);
  bool onMouseScrolled(libs::events::MouseScrolledEvent &event);

  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::Shader m_shaderCube;
  libs::renderer::Shader m_shaderLight;

  unsigned int m_vaoCube, m_vaoLight, m_vbo, m_ebo;
  double m_deltaTime;
  float m_aspectRatio{800.0 / 600.0};
};