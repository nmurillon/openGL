#pragma once

#include <filesystem>

#include <libs/core/Viewport.hpp>
#include <libs/events/Event.hpp>
#include <libs/renderer/BufferLayout.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/MouseCameraController.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/UniformBuffer.hpp>

#include <string>
#include <vector>

class UniformBufferViewport : public libs::core::Viewport {
public:
  virtual ~UniformBufferViewport() = default;
  UniformBufferViewport(
      const std::string &name, float width = 800.f, float height = 600.f,
      const std::filesystem::path &assetsDir = std::filesystem::path{});

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
  void drawRearView();
  void onViewportResize(float newWidth, float newHeight) override;

  // clang-format off
  const std::vector<float> m_vertices{
        -0.5f, -0.5f, 0.5f, // near bottom left
        -0.5f, 0.5f, 0.5f,  // near top left
        0.5f,  0.5f, 0.5f, // near top right
        0.5f, -0.5f, 0.5f, // near bottom right

        -0.5f, -0.5f, -0.5f, // far bottom left
        -0.5f, 0.5f, -0.5f,  // far top left
        0.5f,  0.5f, -0.5f, // far top right
        0.5f, -0.5f, -0.5f, // far bottom right
  };

  const std::vector<unsigned int> m_indexes{
    // front
    0, 1, 2,
    0, 2, 3,
    // back
    4, 5, 6,
    4, 6, 7,
    // top
    1, 2, 5,
    2, 5, 6,
    // bottom
    0,3,4,
    3,4,7,
    // right
    2,3,6,
    3,6,7,
    // left
    0,1,4,
    1,4,5
  };
  // clang-format on

  libs::renderer::Cube m_cube{
      libs::renderer::BufferLayout{
          libs::renderer::BufferLayoutElement(sizeof(float), 3, GL_FLOAT),
      },
      m_vertices, m_indexes};

  libs::renderer::UniformBuffer m_uniformBuffer{2 * sizeof(glm::mat4)};

  constexpr static const std::array<std::string, 4> s_colors = {
      "red", "green", "blue", "yellow"};

  constexpr static const std::array<glm::vec3, 4> s_positions = {
      glm::vec3(-0.75f, 0.75f, 0.0f), glm::vec3(0.75f, 0.75f, 0.0f),
      glm::vec3(0.75f, -0.75f, 0.0f), glm::vec3(-0.75f, -0.75f, 0.0f)};
};