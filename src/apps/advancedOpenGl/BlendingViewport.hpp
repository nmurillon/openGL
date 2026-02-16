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

class BlendingViewport : public libs::core::Viewport {
public:
  BlendingViewport(const std::string &name, float width = 800.f,
                   float height = 600.f);

public:
  virtual ~BlendingViewport() = default;

  virtual void onImguiUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;

protected:
  virtual void drawScene() override;

private:
  void drawFloor();
  void drawCube(const glm::vec3 &position);
  void drawTransparent();

  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::MouseCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};

  libs::renderer::Texture m_marble{libs::renderer::TextureType::DIFFUSE,
                                   std::string{}};
  libs::renderer::Texture m_metal{libs::renderer::TextureType::DIFFUSE,
                                  std::string{}};
  libs::renderer::Texture m_transparent{libs::renderer::TextureType::DIFFUSE,
                                        std::string{}};

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

    const std::vector<float> m_transparentVertices{
        // positions          // texture Coords
         0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f,

         0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.0f,  1.0f,  0.0f,  0.0f, 1.0f								
    };
  // clang-format on

  libs::renderer::Cube m_cube{libs::renderer::Cube::DataType::POSITION_TEXTURE,
                              m_cubeVertices};

  libs::renderer::Cube m_floor{libs::renderer::Cube::DataType::POSITION_TEXTURE,
                               m_planeVertices};

  libs::renderer::Cube m_transparentCube{
      libs::renderer::Cube::DataType::POSITION_TEXTURE, m_transparentVertices};

  std::vector<glm::vec3> m_positions{{-1.5f, -0.5f, -0.48f},
                                     {1.5f, -0.5f, 0.51f},
                                     {0.f, -0.5f, 0.7f},
                                     {-0.3f, -0.5f, -2.3f},
                                     {0.5f, -0.5f, -0.6f}};
};