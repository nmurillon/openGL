#pragma once

#include <filesystem>

#include <libs/core/Viewport.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <libs/renderer/VertexArray.hpp>

#include <string>

class HousesViewport : public libs::core::Viewport {
public:
  virtual ~HousesViewport() = default;
  HousesViewport(
      const std::string &name, float width = 800.f, float height = 600.f,
      const std::filesystem::path &assetsDir = std::filesystem::path{});

  void onEvent(libs::events::Event &event) override {};

private:
  std::filesystem::path m_assetsDir;
  libs::renderer::ShaderManager m_shaderManager{};

protected:
  void drawScene() override;
  void initState() override;
  void resetState() override;

  libs::renderer::VertexArray m_vao{};
};