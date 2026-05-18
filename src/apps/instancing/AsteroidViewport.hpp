#pragma once

#include <libs/core/Viewport.hpp>
#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Cube.hpp>
#include <libs/renderer/FlyCameraController.hpp>
#include <libs/renderer/Model.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <filesystem>
#include <memory>

class AsteroidViewport : public libs::core::Viewport {
public:
  AsteroidViewport(const std::string &name, float width = 800.f,
                   float height = 600.f, std::filesystem::path assetsDir = {});
  virtual ~AsteroidViewport() = default;

  virtual void onEvent(libs::events::Event &event) override {
    m_cameraController.onEvent(event);
  };

protected:
  void drawScene() override;
  void initState() override;
  void resetState() override;

private:
  void drawPlanet();
  void drawAsteroids();

private:
  std::filesystem::path m_assetsDir{};
  libs::renderer::ShaderManager m_shaderManager{};
  std::shared_ptr<libs::renderer::Camera> m_camera;
  libs::renderer::FlyCameraController m_cameraController;

  std::shared_ptr<libs::renderer::Model> m_planet{nullptr};
  std::shared_ptr<libs::renderer::Model> m_asteroid{nullptr};
};