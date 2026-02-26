#pragma once

#include <libs/core/Viewport.hpp>
#include <libs/events/Event.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/MouseCameraController.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/Texture.hpp>

#include <filesystem>
#include <memory>

class FaceCullingViewport : public libs::core::Viewport {
public:
  virtual ~FaceCullingViewport() = default;
  FaceCullingViewport(
      const std::string &name, float width = 800.f, float height = 600.f,
      const std::filesystem::path &assetsDir = std::filesystem::path{});

  //   void onImguiUpdate() override;
  void onEvent(libs::events::Event &event) override;

protected:
  void drawScene() override;

private:
  std::filesystem::path m_assetsDir;
  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::MouseCameraController m_cameraController;
  libs::renderer::ShaderManager m_shaderManager{};
  libs::renderer::Texture m_metal{libs::renderer::TextureType::DIFFUSE,
                                  std::string{}};

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
    //clang-format on

    libs::renderer::Cube m_cube{libs::renderer::Cube::DataType::POSITION_TEXTURE,
                              m_cubeVertices};
};