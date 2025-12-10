#pragma once

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
#include <vector>

class MultiLightsLayer : public libs::core::Layer {

  struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
  };

  static constexpr DirectionalLight s_defaultDirectionalLight{
      glm::vec3{-0.2f, -1.0f, -0.3f}, glm::vec3{1.f, 1.f, 1.f},
      glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.4f, 0.4f, 0.4f},
      glm::vec3{0.5f, 0.5f, 0.5f}};

  struct PointLight {
    glm::vec3 position;
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
  };

  struct Spotlight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;
  };

public:
  MultiLightsLayer(const std::string &name = "MultiLightsLayer");

  virtual ~MultiLightsLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;
  virtual void onImguiUpdate() override;

private:
  bool onWindowResized(libs::events::WindowResizeEvent &event);

  void updateCubes();
  void updateLights();

  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::FlyCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};

  unsigned int m_vaoCube, m_vaoLight, m_vbo, m_ebo;
  double m_deltaTime;
  float m_aspectRatio{800.0 / 600.0};

  int m_diffuseMap;
  int m_specularMap;

  DirectionalLight m_directionalLight{s_defaultDirectionalLight};

  std::vector<PointLight> m_pointLights{};
  static constexpr int s_maxPointLights = 4;

  static constexpr PointLight s_defaultPointLight{
      glm::vec3(1.2f, 1.0f, 2.0f),
      glm::vec3(1.f),
      glm::vec3(0.2f),
      glm::vec3(0.5f),
      glm::vec3(1.f),
      1.0f,
      0.09f,
      0.032f,
  };
};