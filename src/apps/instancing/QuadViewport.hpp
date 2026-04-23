#pragma once

#include <libs/core/Viewport.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/Model.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <filesystem>

class QuadViewport : public libs::core::Viewport {
public:
  QuadViewport(const std::string &name, float width = 800.f,
               float height = 600.f, std::filesystem::path assetsDir = {});
  virtual ~QuadViewport() = default;

  virtual void onEvent(libs::events::Event &event) override {};

protected:
  void drawScene() override;
  void initState() override {};
  void resetState() override {};

private:
  std::filesystem::path m_assetsDir{};
  libs::renderer::ShaderManager m_shaderManager{};
  libs::renderer::Cube m_quad{
      libs::renderer::BufferLayout{
          libs::renderer::BufferLayoutElement{sizeof(float), 2},
          libs::renderer::BufferLayoutElement{sizeof(float), 3}},
      std::vector<float>{
          // clang-format off
          // positions              // colors
          -0.05f, 0.05f, 1.0f,   0.0f,   0.0f,
          0.05f, -0.05f, 0.0f,  1.0f,   0.0f,
          -0.05f, -0.05f, 0.0f, 0.0f,  1.0f,

          -0.05f, 0.05f, 1.0f,   0.0f,   0.0f,
          0.05f, -0.05f, 0.0f, 1.0f,   0.0f,  
          0.05f,  0.05f,  0.0f, 1.0f,  1.0f
          // clang-format on
      }};
};