#pragma once

#include <filesystem>

#include <libs/core/Viewport.hpp>
#include <libs/events/Event.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/MouseCameraController.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/Texture.hpp>

class FrameBufferViewport : public libs::core::Viewport {
public:
  virtual ~FrameBufferViewport() = default;
  FrameBufferViewport(
      const std::string &name, float width = 800.f, float height = 600.f,
      const std::filesystem::path &assetsDir = std::filesystem::path{});

  void onImguiUpdate() override;
  void onEvent(libs::events::Event &event) override;

private:
  std::filesystem::path m_assetsDir;
  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::MouseCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};

protected:
  void drawScene() override;

private:
  void drawInFrameBuffer();
  bool onViewportResize(float newWidth, float newHeight) override;

  // clang-format off
  const std::vector<float> m_cubeVertices{
        // positions                        // texture Coords
        // back face
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top right

         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,// top left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom left

        // front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top right

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom left

        // left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom left

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top right

        // right face
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom right

         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top left

         // bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom left
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // bottom right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // top right

         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // top right
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // top left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom left

        // top face
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom right

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom right
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f // bottom left
    };
  // clang-format on

  libs::renderer::Cube m_cube{libs::renderer::Cube::DataType::POSITION_TEXTURE,
                              m_cubeVertices};
  libs::renderer::Texture m_metal{800, 600};

  // Everything related to the framebuffer, textures and renderbuffer
  // TODO: move this to a FrameBuffer class
private:
  unsigned int m_frameBuffer{};
  unsigned int m_renderBuffer{};
  libs::renderer::Texture m_textureColorBuffer{800, 600};
  libs::renderer::Cube m_quad{
      libs::renderer::Cube::DataType::POSITION2D_TEXTURE,
      {
          // clang-format off
        -1.f, -1.f, 0.f, 0.f,
        1.f, -1.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        -1.f, 1.f, 0.f, 1.f,
          // clang-format on
      },
      {0, 1, 2, 0, 2, 3}};

  std::string m_currentShader{"framebuffer"};
};