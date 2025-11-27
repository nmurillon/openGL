#pragma once

#include <libs/core/Layer.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>

class MaterialsLayer : public libs::core::Layer {
public:
  MaterialsLayer(const std::string &name = "MaterialsLayer");

  virtual ~MaterialsLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;

private:
  bool onWindowResized(libs::events::WindowResizeEvent &event);
  bool onKeyPressed(libs::events::KeyPressedEvent &event);
  bool onMouseMoved(libs::events::MouseMouvedEvent &event);
  bool onMouseScrolled(libs::events::MouseScrolledEvent &event);

  void updateShaderCube();
  void updateShaderLight();

  std::shared_ptr<libs::renderer::Camera> m_camera;
  glm::vec3 m_lightPos;
  glm::vec3 m_lightColor{1.f, 1.f, 1.f};
  libs::renderer::ShaderManager m_shaderManager{};

  unsigned int m_vaoCube, m_vaoLight, m_vbo, m_ebo;
  double m_deltaTime;
  float m_aspectRatio{800.0 / 600.0};
};