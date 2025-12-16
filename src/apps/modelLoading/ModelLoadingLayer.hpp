#pragma once

#include <libs/core/Layer.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/FlyCameraController.hpp>
#include <libs/renderer/Model.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

class ModelLoadingLayer : public libs::core::Layer {

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
  ModelLoadingLayer(const std::string &name = "ModelLoadingLayer");

  virtual ~ModelLoadingLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;
  virtual void onImguiUpdate() override;

private:
  bool onWindowResized(libs::events::WindowResizeEvent &event);
  void onPathDropped(libs::events::PathDropEvent &event);

  void updateShaderLight();

  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::FlyCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};
  std::map<std::string, libs::renderer::Model> m_models;

  unsigned int m_vaoLight, m_vbo, m_ebo;
  double m_deltaTime;
  float m_aspectRatio{800.0 / 600.0};

  Light m_light{defaultLight};

  const std::vector<unsigned int> indices = {
      0, 1, 2, 1, 2, 3, // Up face
      0, 2, 4, 2, 4, 6, // Left face
      0, 1, 4, 1, 4, 5, // Far face
      1, 3, 5, 3, 5, 7, // Right face
      2, 3, 6, 3, 6, 7, // Near face
      4, 5, 6, 5, 6, 7, // Down face
  };

  const std::vector<float> vertices = {
      // Up face
      -0.5f, 0.5f, 0.5f,  // TOP-LEFT-UP
      0.5f, 0.5f, 0.5f,   // TOP-RIGHT-UP
      -0.5f, -0.5f, 0.5f, // BOTTOM-LEFT-UP
      0.5f, -0.5f, 0.5f,  // BOTTOM-RIGHT-UP
      // DOWN face

      -0.5f, 0.5f, -0.5f,  // TOP-LEFT-DOWN
      0.5f, 0.5f, -0.5f,   // TOP-RIGHT-DOWN
      -0.5f, -0.5f, -0.5f, // BOTTOM-LEFT-DOWN
      0.5f, -0.5f, -0.5f,  // BOTTOM-RIGHT-DOWN
  };
};