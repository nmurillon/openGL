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

class LightCasterLayer : public libs::core::Layer {

  const std::vector<std::string> lightTypes{"Directional Light", "Point Light",
                                            "Spotlight"};

  struct Light {
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

  const Light defaultLight{glm::vec3(1.2f, 1.0f, 2.0f),
                           glm::vec3(-0.2f, -1.f, -0.3f),
                           glm::vec3(1.f, 1.f, 1.f),
                           glm::vec3(0.2f),
                           glm::vec3(0.5f),
                           glm::vec3(1.f),
                           1.0f,
                           0.09f,
                           0.032f,
                           12.5f,
                           17.5f};

public:
  LightCasterLayer(const std::string &name = "LightCasterLayer");

  virtual ~LightCasterLayer() = default;

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

  int m_diffuseMap;
  int m_specularMap;

  std::string m_currentLightType{lightTypes[0]};

  Light m_light{defaultLight};
};