#pragma once

#include <filesystem>

#include <libs/core/Viewport.hpp>
#include <libs/events/Event.hpp>
#include <libs/renderer/BufferLayout.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/MouseCameraController.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/Texture.hpp>

#include <map>
#include <string>
#include <vector>

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
  void initState() override;
  void resetState() override;

private:
  void drawInFrameBuffer();
  void onViewportResize(float newWidth, float newHeight) override;

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

  libs::renderer::Cube m_cube{
      libs::renderer::BufferLayout{
          libs::renderer::BufferLayoutElement(sizeof(float), 3, GL_FLOAT),
          libs::renderer::BufferLayoutElement(sizeof(float), 2, GL_FLOAT),
      }

      ,
      m_cubeVertices};
  libs::renderer::Texture m_wood{800, 600};

  // Everything related to the framebuffer, textures and renderbuffer
  // TODO: move this to a FrameBuffer class
private:
  unsigned int m_frameBuffer{};
  unsigned int m_renderBuffer{};
  libs::renderer::Texture m_textureColorBuffer{800, 600};
  libs::renderer::Cube m_quad{
      libs::renderer::BufferLayout{
          libs::renderer::BufferLayoutElement(sizeof(float), 2, GL_FLOAT),
          libs::renderer::BufferLayoutElement(sizeof(float), 2, GL_FLOAT),
      },
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
  const static std::map<std::string, std::vector<float>> s_kernels;
  std::vector<float> m_kernel{s_kernels.at("custom")};
};