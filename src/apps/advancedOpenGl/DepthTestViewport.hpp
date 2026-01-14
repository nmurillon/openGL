#include <libs/core/Viewport.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/FlyCameraController.hpp>
#include <libs/renderer/Model.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/Texture.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>

class DepthTestViewport : public libs::core::Viewport {
public:
  DepthTestViewport(const std::string &name, float width = 800.f,
                    float height = 600.f);

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

public:
  virtual ~DepthTestViewport() = default;

  virtual void onImguiUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;

protected:
  virtual void drawScene() override;

private:
  unsigned int m_cubeVAO, m_planeVAO, m_cubeVBO, m_planeVBO;

  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::FlyCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};

  libs::renderer::Texture m_marble{libs::renderer::TextureType::DIFFUSE,
                                   std::string{}};
  libs::renderer::Texture m_metal{libs::renderer::TextureType::DIFFUSE,
                                  std::string{}};
};