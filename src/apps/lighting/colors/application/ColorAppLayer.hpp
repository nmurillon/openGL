#pragma once

#include <libs/core/Layer.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/FlyCameraController.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>

enum class LightType { Ambient, Diffuse, Specular, SpecularViewSpace, Gouraud };

std::string lightTypeToString(LightType type);

class ColorAppLayer : public libs::core::Layer {
public:
  ColorAppLayer(const std::string &name = "ColorAppLayer");

  virtual ~ColorAppLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;

private:
  bool onWindowResized(libs::events::WindowResizeEvent &event);

  void updateShaderCube();
  void updateShaderLight();

  std::shared_ptr<libs::renderer::PerspectiveCamera> m_camera;
  libs::renderer::FlyCameraController m_cameraController;
  glm::vec3 m_lightPos;
  LightType m_lightType{LightType::Gouraud};
  libs::renderer::ShaderManager m_shaderManager{};

  unsigned int m_vaoCube, m_vaoLight, m_vbo, m_ebo;
  double m_deltaTime;
  float m_aspectRatio{800.0 / 600.0};
};