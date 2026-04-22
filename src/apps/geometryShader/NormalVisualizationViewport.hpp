#pragma once

#include <libs/core/Viewport.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Model.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/UniformBuffer.hpp>

#include <filesystem>
#include <optional>

class NormalVisualizationViewport : public libs::core::Viewport {
public:
  virtual ~NormalVisualizationViewport() = default;
  NormalVisualizationViewport(
      const std::string &name, float width = 800.f, float height = 600.f,
      const std::filesystem::path &assetsDir = std::filesystem::path{});

  void onEvent(libs::events::Event &event) override;
  void onImguiUpdate() override;

private:
  void onPathDropped(libs::events::PathDropEvent &event);
  void rotateCamera(float time);

private:
  std::filesystem::path m_assetsDir;
  libs::renderer::ShaderManager m_shaderManager{};
  std::shared_ptr<libs::renderer::Camera> m_camera;
  std::optional<libs::renderer::Model> m_model{};
  libs::renderer::UniformBuffer m_uniformBuffer{3 * sizeof(glm::mat4)};

protected:
  void drawScene() override;
  void initState() override;
  void resetState() override;
  void onViewportResize(float newWidth, float newHeight) override;

private:
  float m_magnitude{0.4f};
  glm::vec4 m_color{1.0, 1.0, 0.0, 1.0};
};