#include <libs/core/Viewport.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/Model.hpp>
#include <libs/renderer/MouseCameraController.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/Texture.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <vector>

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
  void drawFloor();
  void drawCube(const glm::vec3 &position);

  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::MouseCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};

  libs::renderer::Texture m_marble{libs::renderer::TextureType::DIFFUSE,
                                   std::string{}};
  libs::renderer::Texture m_metal{libs::renderer::TextureType::DIFFUSE,
                                  std::string{}};

  bool m_showDepthBuffer{false};
  bool m_showOutline{true};
  bool m_showGroupedOutline{false};

  glm::vec3 m_outlineColor{1.0f, 0.0f, 0.0f};
  float m_outlineScale{1.025f};

  int m_depthFunc = GL_LESS;
  const std::vector<std::string> m_depthFuncs{
      "GL_LESS",    "GL_NEVER",    "GL_EQUAL",  "GL_LEQUAL",
      "GL_GREATER", "GL_NOTEQUAL", "GL_GEQUAL", "GL_ALWAYS"};

  // clang-format off
  const std::vector<float> m_cubeVertices{
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    const std::vector<float> m_planeVertices{
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
    };
  // clang-format on

  libs::renderer::Cube m_cube{libs::renderer::Cube::DataType::POSITION_TEXTURE,
                              m_cubeVertices};

  libs::renderer::Cube m_floor{libs::renderer::Cube::DataType::POSITION_TEXTURE,
                               m_planeVertices};
};