#pragma once

#include "Materials.hpp"

#include <libs/core/Layer.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/FlyCameraController.hpp>
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
  virtual void onImguiUpdate() override;

private:
  bool onWindowResized(libs::events::WindowResizeEvent &event);

  void updateShaderCube();
  void updateShaderLight();

  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::FlyCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};

  unsigned int m_vaoCube, m_vaoLight, m_vbo, m_ebo;
  double m_deltaTime;
  float m_aspectRatio{800.0 / 600.0};

  Material m_material{tutorial};
  Light m_light{defaultLight};
};